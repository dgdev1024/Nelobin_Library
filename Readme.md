# Nelobin Binary File Format - Version 1.0
## Introduction
**Nelobin** is a binary serialization and de-serialization library written in C++. This library is intended for storing data and other configurations in a registry-esque binary format, and is quite inspired by The Cherno's Java binary serialization tutorials (https://www.youtube.com/playlist?list=PLlrATfBNZ98cCfmH0xPebdVVMSYRQfyKi).

## Usage Example
### Creating a Nelobin Registry File
```c++
#include <Nelobin/Registry.hpp>

int main ()
{
    // Create a Nelobin registry object.
    Nelobin::Registry l_registry;

    // Add a new key to that registry and store a handle to that key.
    auto& l_developer = l_registry.addKey("Developer");

    // Add some data to the key we just created. This data is organized
    // into key-value pairs.
    l_developer.addEntry<std::string>("Name").set("Dennis Griffin");
    l_developer.addEntry<std::string>("Favorite Language").set("C++");

    // Keys can also have other subkeys stored in them, too.
    auto& l_instrument = l_developer.addKey("Instrument");
    l_instrument.addEntry<std::string>("Type").set("Guitar");
    l_instrument.addEntry<std::string>("Name").set("Epiphone Explorer");
    l_instrument.addEntry<short>("Number of Strings").set(6);
    l_instrument.addEntry<unsigned char>("Is Electric").set(0x01);  // Represents a boolean 'true'.

    // The registry object itself also acts as a key, which stores entries and subkeys.
    l_registry.addEntry<float>("Gravity").set(9.81f);

    // Now save the file.
    l_registry.saveToFile("Person.nbf");

    return 0;
}
```
### Loading a Nelobin Registry File...
```c++
#include <iostream>
#include <Nelobin/Registry.hpp>

int main ()
{
    // Create the registry object.
    Nelobin::Registry l_registry;

    // Attempt to load the registry file...
    Nelobin::Status l_status = l_registry.loadFromFile("Person.nbf");
    if (l_status == Nelobin::Status::OK)
    {
        // Fetch the developer's name and favorite instrument.
        const auto& l_developer = l_registry.getKey("Developer");
        const auto& l_instrument = l_developer.getKey("Instrument");

        std::cout << l_developer.getEntry<std::string>("Name").get() << std::endl;
        std::cout << l_instrument.getEntry<std::string>("Name").get() << std::endl;
    }
    else
    {
        // If an error occurs, we can get a string form of the status code
        // that was returned.
        std::cerr << Nelobin::stringifyStatus(l_status) << std::endl;
    }

    return 0;
}
```

Nelobin registry files shall have the filename extension ".nbf".

## The Nelobin Buffer
The **Nelobin::Buffer** class is the lowest-level of the Nelobin serialization library. All data in the library is serialized to and deserialized from objects of this class. The buffer can read and write the following data types:

* 8-bit integers
    * `signed char`
    * `unsigned char`
    * `std::int8_t`
    * `std::uint8_t`
    * `Nelobin::S8`
    * `Nelobin::U8`
* 16-bit integers
    * `short`
    * `unsigned short`
    * `std::int16_t`
    * `std::uint16_t`
    * `Nelobin::S16`
    * `Nelobin::U16`
* 32-bit integers
    * `int`
    * `unsigned int`
    * `std::int32_t`
    * `std::uint32_t`
    * `Nelobin::S32`
    * `Nelobin::U32`
* `float` and `Nelobin::Float`
* `std::string` and `Nelobin::String`

On account of the fact that C++ has a special template specialization for `std::vector<bool>`, Nelobin is not yet able to serialize or deserialize values of type `bool`. To get around this, you can use one of the integer types - preferably 8-bit to save space - like this:
```c++
#include <Nelobin/Buffer.hpp>

Nelobin::Buffer l_buffer;
l_buffer.writeData<unsigned char>("Boolean").set(0x01);
```

Here is an example of the Nelobin buffer in use:
```c++
#include <iostream>
#include <Nelobin/Buffer.hpp>

int main ()
{
    Nelobin::Buffer l_bufferOne, l_bufferTwo;

    l_bufferOne.writeData<std::string>("Dennis Griffin")
               .writeData<short>(25);

    l_bufferOne.saveToFile("Dennis.bin");

    Nelobin::Status l_status = l_bufferTwo.loadFromFile("Dennis.bin");
    if (l_status == Nelobin::Status::OK)
    {
        std::string l_name = "";
        short l_age = 0;

        l_bufferTwo.readData<std::string>(l_name)
                   .readData<short>(l_age);

        std::cout << l_name << ", " << l_age << std::endl;
    }

    return 0;
}
```

## Entries
Data **entries** in the Nelobin library are organized into key-value pairs inside of containers called **keys**. The registry object itself also acts as a key.

Fields can be added to a host key, then a handle retrieved, like so:
```c++
auto& l_entry = l_key.addEntry<int>("Entry Name");
```

Fields can be obtained from a host key like this:
```c++
auto& l_entry = l_key.getEntry<int>("Entry Name");
```

The entry's value can be get and set like this:
```c++
int l_value = l_entry.get();
l_entry.set(25);
```

If an entry with a given name is not found inside of a key, then a "null" entry will be returned to the user. This "null" entry has no effect on the key or registry in question if modified. You can check to see if an entry retrieved is "null" by using the `isNull` getter:
```c++
if (l_entry.isNull() == true)
    std::cerr << "This entry is null. Perhaps the entry we wanted was not found?" << std::endl;
```

## Keys
The Nelobin **Key** class is capable of storing data entries and other subkeys. The Nelobin registry object is also a key; it inherits from the key class. A key can be added to a registry object or parent key like so:
```c++
Nelobin::Registry l_registry;
Nelobin::Key& l_key = l_registry.addKey("Name of Key");
Nelobin::Key& l_subkey = l_key.addKey("Name of Subkey");
```

Keys can be retrieved as follows:
```c++
auto& l_key = l_registry.getKey("Name of Key");
auto& l_subkey = l_key.getKey("Name of Subkey");
```

It is also possible for multiple keys with the same name to be stored in the same parent key.
```c++
l_file.addKey("Key");
l_file.addKey("Key");
l_file.addKey("Key");

for (unsigned int i = 0; i < l_file.getKeyCount("Key"); ++i)
{
    auto& l_key = l_file.getKey("Key", i);
}
```

As seen above, multiple keys in the same parent key can have the same name. Internally, they have a unique ID number, called an "instance". These ID numbers are refreshed every time a key is added, renamed, or removed, so they can be easily iterated through.

As with entries, if a key with a given name is not found in the parent key, a "null" key is returned which has no effect on the key in question.

## How to compile...
A CMakeLists.txt file is included with this repository, so you can generate your own project files via CMake in order to build Nelobin. Additionally, a CMake find module is included so you can easily include this dependency in your project if you are using CMake to generate build files. Both files are heavily commented so you can follow both and get a better idea of how to use CMake in your project.

If there are any problems with these CMake scripts, or any way to better them, please let me know.
