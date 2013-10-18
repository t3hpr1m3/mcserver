#include <jni.h>
#include <iostream>

#include "dynamiclib.h"

extern "C" typedef jint (*JNI_CreateJavaVM_t)(JavaVM**, void*, JavaVMInitArgs*);

int main(int argc, char *argv[]) {
	JavaVM *jvm;
	JNIEnv *env;
	JavaVMInitArgs vm_args;
	JavaVMOption* options = new JavaVMOption[1];
	options[0].optionString = const_cast<char*>("-Djava.class.path=/usr/lib/java");
	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 1;
	vm_args.options = options;
	vm_args.ignoreUnrecognized = false;

	MCServer::DynamicLib jLib;
	if (!jLib.Load("/usr/lib64/icedtea7/lib/server", "libjvm.so")) {
		std::cout << "Unable to load JVM" << std::endl;
		return -1;
	}

	void *ptr = jLib.GetSymbol("JNI_CreateJavaVM");
	if (ptr == NULL) {
		std::cerr << "Unable to find JNI_CreateJavaVM" << std::endl;
		return -1;
	}

	JNI_CreateJavaVM_t create = reinterpret_cast<JNI_CreateJavaVM_t>(reinterpret_cast<size_t>(ptr));
	//void *_JNI_CreateJavaVM = jLib.GetSymbol("JNI_CreateJavaVM");

	(*create)(&jvm, (void**)&env, &vm_args);
	delete options;
	jclass cls = env->FindClass("Main");
	jmethodID mid = env->GetStaticMethodID(cls, "test", "(I)V");
	env->CallStaticVoidMethod(cls, mid, 100);
	jvm->DestroyJavaVM();

	return 0;
}
