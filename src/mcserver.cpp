#include <config.h>
#include <globals.h>

#include <jni.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>

#include "dynamiclib.h"

#define IGNORE_RETURN(fn) if (fn)

extern "C" typedef jint (*JNI_CreateJavaVM_t)(JavaVM**, void*, JavaVMInitArgs*);

void getWorkingDir(std::string& pPath, const char* pProg) {
	char vBuffer[PATH_MAX];
	IGNORE_RETURN(getcwd(vBuffer, PATH_MAX));
	std::string vProgramName = vBuffer;
	vProgramName += "/";
	vProgramName += pProg;
	std::vector<std::string> vChunks;
	std::string vTemp;
	std::istringstream iss(vProgramName);
	while (std::getline(iss, vTemp, '/')) {
		if (vTemp.compare("..") == 0) {
			vChunks.pop_back();
		} else if (vTemp.compare(".") != 0 && vTemp.length() > 0) {
			vChunks.push_back(vTemp);
		}
	}

	vChunks.pop_back(); // Delete the actual program name
	for (std::vector<std::string>::const_iterator vIt = vChunks.begin(); vIt != vChunks.end(); vIt++) {
		pPath += "/";
		pPath += *vIt;
	}
}

MCServer::DynamicLib* loadVM() {
	std::string jLibPath = std::getenv("JAVA_HOME");
	jLibPath += "/jre/lib/";
#if MCSERVER_ARCH == ARCH_32
	jLibPath += "i386";
#elif MCSERVER_ARCH == ARCH_64
	jLibPath += "amd64";
#endif
	jLibPath += "/server/";
	dlerror();

	MCServer::DynamicLib *jLib = new MCServer::DynamicLib();
	if (!jLib->Load(jLibPath, "jvm")) {
		std::cerr << "Error: " << jLib->Error() << std::endl;
		delete jLib;
		std::cerr << "Unable to load JVM" << std::endl;
		return NULL;
	}
	return jLib;
}


int main(int argc, char *argv[]) {
	std::string vJarPath;
	getWorkingDir(vJarPath, argv[0]);
	std::cout << "Working dir: " << vJarPath << std::endl;
	vJarPath += "/minecraft_server.1.6.2.jar";
	std::string vClassPath = const_cast<char*>("-Djava.class.path=/usr/lib/java:");
	vClassPath += vJarPath;
	JavaVM *jvm = NULL;
	JNIEnv *env = NULL;
	JavaVMInitArgs vm_args;
	JavaVMOption* options = new JavaVMOption[1];
	options[0].optionString = const_cast<char*>(vClassPath.c_str());
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

	JNI_CreateJavaVM_t create = reinterpret_cast<JNI_CreateJavaVM_t>(reinterpret_cast<size_t>(ptr));
	//void *_JNI_CreateJavaVM = jLib.GetSymbol("JNI_CreateJavaVM");

	(*create)(&jvm, (void**)&env, &vm_args);
	delete options;
	jclass cls = env->FindClass("net/minecraft/server/MinecraftServer");
	if (cls == NULL) {
		std::cerr << "Unable to find Main class" << std::endl;
		jLib->UnLoad();
		delete jLib;
		return -1;
	}
	jmethodID mid = env->GetStaticMethodID(cls, "main", "([Ljava/lang/String;)V");
	if (mid == NULL) {
		std::cerr << "Unable to find method" << std::endl;
		jvm->DestroyJavaVM();
		jLib->UnLoad();
		delete jLib;
		return -1;
	}
	jclass stringClass = env->FindClass("java/lang/String");
	if (stringClass == NULL) {
		std::cerr << "Unable to find String class" << std::endl;
		jvm->DestroyJavaVM();
		jLib->UnLoad();
		delete jLib;
		return -1;
	}

	jobjectArray vArgs = env->NewObjectArray(1, stringClass, NULL);
	jstring vStringArg = env->NewStringUTF("--nogui");
	env->SetObjectArrayElement(vArgs, 0, vStringArg);
	env->CallStaticVoidMethod(cls, mid, vArgs);
	jvm->DestroyJavaVM();

	return 0;
}
