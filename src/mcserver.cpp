#include <config.h>
#include <globals.h>

#include <jni.h>
#include <iostream>
#include <cstdlib>

#include "dynamiclib.h"

extern "C" typedef jint (*JNI_CreateJavaVM_t)(JavaVM**, void*, JavaVMInitArgs*);

MCServer::DynamicLib* loadVM() {
	std::string jLibPath = std::getenv("JAVA_HOME");
	jLibPath += "/jre/lib/";
#if MCSERVER_ARCH == ARCH_32
	jLibPath += "i386";
#elif MCSERVER_ARCH == ARCH_64
	jLibPath += "x86_64";
#endif
	jLibPath += "/server/";
	std::cout << "jLibPath: " << jLibPath << std::endl;
	dlerror();

	MCServer::DynamicLib *jLib = new MCServer::DynamicLib();
	if (!jLib->Load(jLibPath, "jvm")) {
		std::cerr << "Error: " << jLib->Error() << std::endl;
		delete jLib;
		std::cerr << "Unable to load JVM" << std::endl;
		return NULL;
	}
	std::cout << "JVM Loaded" << std::endl;
	return jLib;
}


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

	MCServer::DynamicLib* jLib = loadVM();
	if (jLib == NULL)
		return -1;

	void *ptr = jLib->GetSymbol("JNI_CreateJavaVM");
	if (ptr == NULL) {
		std::cerr << "Unable to find JNI_CreateJavaVM" << std::endl;
		return -1;
	}

	std::cout << "Pointer found" << std::endl;

	JNI_CreateJavaVM_t create = reinterpret_cast<JNI_CreateJavaVM_t>(reinterpret_cast<size_t>(ptr));
	//void *_JNI_CreateJavaVM = jLib.GetSymbol("JNI_CreateJavaVM");

	(*create)(&jvm, (void**)&env, &vm_args);
	std::cout << "VM Created" << std::endl;
	delete options;
	jclass cls = env->FindClass("Main");
	if (cls == NULL) {
		std::cerr << "Unable to find Main class" << std::endl;
		jLib->UnLoad();
		delete jLib;
		return -1;
	}
	jmethodID mid = env->GetStaticMethodID(cls, "test", "(I)V");
	env->CallStaticVoidMethod(cls, mid, 100);
	jvm->DestroyJavaVM();

	return 0;
}
