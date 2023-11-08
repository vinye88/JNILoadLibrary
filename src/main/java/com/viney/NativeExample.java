package com.viney;

public class NativeExample {
    static
    {
        System.loadLibrary("nativeExample"); // Carrega a biblioteca nativa
    }

    // Declaração do método nativo
    private native int addNumbers(int a, int b);
    public native void modifyPerson(Person person);

    public static void main(String[] args)
    {
        NativeExample example = new NativeExample();
        int result = example.addNumbers(5, 7);
        System.out.println("Resultado: " + result);
        UsePerson(example);
    }

    public static void UsePerson(NativeExample example)
    {
        Address address = new Address();
        address.city = "Sample City";
        address.postalCode = 12345;

        Person person = new Person();
        person.name = "John Doe";
        person.age = 30;
        person.address = address;

        System.out.println("Before modification:");
        System.out.println("Name: " + person.name);
        System.out.println("Age: " + person.age);
        System.out.println("City: " + person.address.city);
        System.out.println("Postal Code: " + person.address.postalCode);

        example.modifyPerson(person);

        System.out.println("After modification:");
        System.out.println("Name: " + person.name);
        System.out.println("Age: " + person.age);
        System.out.println("City: " + person.address.city);
        System.out.println("Postal Code: " + person.address.postalCode);
    }
}
