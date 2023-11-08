#include <algorithm>
#include <iostream>
#include <jni.h>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <string>

#include "com_viney_NativeExample.h"

struct Address {
    char city[50];
    int postalCode;
};

struct Person {
    char name[50];
    int age;
    Address *address;
};

void Log(std::string msg)
{
    std::cout << msg << std::endl;
}

template<typename ... Args>
void Log( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    Log(std::string( buf.get(), buf.get() + size));
}

JNIEXPORT void JNICALL Java_com_viney_NativeExample_modifyPerson(JNIEnv *env, jobject obj, jobject personObj)
{
    try
    {
        Log("> init");
        jclass personClass = env->GetObjectClass(personObj);
        // Access the fields of the Java Person object
        jfieldID nameField = env->GetFieldID(personClass, "name", "Ljava/lang/String;");
        jfieldID ageField = env->GetFieldID(personClass, "age", "I");
        jfieldID addressField = env->GetFieldID(personClass, "address", "Lcom/viney/Address;");

        jstring nameStr = (jstring)env->GetObjectField(personObj, nameField);
        jint age = env->GetIntField(personObj, ageField);
        jobject addressObj = env->GetObjectField(personObj, addressField);
        
        // Access the fields of the Java Address object
        jclass addressClass = env->GetObjectClass(addressObj);
        
        // Get the field IDs of the Address class
        jfieldID cityField = env->GetFieldID(addressClass, "city", "Ljava/lang/String;");
        jfieldID postalCodeField = env->GetFieldID(addressClass, "postalCode", "I");

        jstring cityStr = (jstring)env->GetObjectField(addressObj, cityField);
        jint postalCode = env->GetIntField(addressObj, postalCodeField);
        Log(" got params");

        // Modify the native struct
        Person nativePerson;
        Address sAddress;
        nativePerson.address = &sAddress;

        const char * pName = env->GetStringUTFChars(nameStr, NULL);
        std::string name(pName);
        env->ReleaseStringUTFChars(nameStr, pName);
        std::copy_n(std::begin(name), sizeof(nativePerson.name), nativePerson.name);
        nativePerson.age = age;

        const char * pAddress = env->GetStringUTFChars(cityStr, NULL);
        env->ReleaseStringUTFChars(cityStr, pAddress);
        std::string address(pAddress);
        std::copy_n(std::begin(address), sizeof(nativePerson.address->city), nativePerson.address->city);
        nativePerson.address->postalCode = postalCode;

        Log(" Before - Person {Name='%s', Age=%u, Address {City='%s' PostalCode=%u}}",
            nativePerson.name, nativePerson.age, nativePerson.address->city, nativePerson.address->postalCode);
        
        name +=" Modified";
        std::copy_n(std::begin(name), sizeof(nativePerson.name), nativePerson.name);
        nativePerson.age = age+10;

        address += " Modified";
        std::copy_n(std::begin(address), sizeof(nativePerson.address->city), nativePerson.address->city);
        nativePerson.address->postalCode = postalCode + 1;

        Log(" After - Person {Name='%s', Age=%u, Address {City='%s' PostalCode=%u}}",
            nativePerson.name, nativePerson.age, nativePerson.address->city, nativePerson.address->postalCode);

        // Update the Java object with modified values

        jstring new_name = env->NewStringUTF(nativePerson.name);
        env->SetObjectField(personObj, nameField, new_name);
        env->SetIntField(personObj, ageField, nativePerson.age);

        jstring new_city = env->NewStringUTF(nativePerson.address->city);
        env->SetObjectField(addressObj, cityField, new_city);
        env->SetIntField(addressObj, postalCodeField, nativePerson.address->postalCode);

        //env->SetObjectField(personObj, addressField, addressObj);

        //jclass modifiedAddressClass = env->GetObjectClass(addressObj);
        //jmethodID modifiedAddressConstructor = env->GetMethodID(modifiedAddressClass, "<init>", "()V");
        //jobject modifiedAddressObj = env->NewObject(modifiedAddressClass, modifiedAddressConstructor);

        //env->SetObjectField(personObj, addressField, modifiedAddressObj);
        Log("< finish");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

JNIEXPORT jint JNICALL Java_com_viney_NativeExample_addNumbers(JNIEnv *env, jobject obj, jint a, jint b)
{
    return a + b;
}
