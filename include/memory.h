#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <string>
#include <windows.h>

typedef unsigned long long address_t;
typedef unsigned long process_id_t;

/**
 * @brief Attached process pid
 */
extern process_id_t g_process_id;

/**
 * @brief Attached module m_address
 */
extern address_t g_module_address;

namespace process {

    /**
     * @brief Returns target process id
     */
    process_id_t get_process_id(std::string process_name);

    /**
     * @brief Returns m_address of the requested module in attached process
     */
    address_t get_module_address(std::wstring module_name);
}

namespace memory {

    /**
     * @brief Initializes memory connection - each with his own implementation
     */
    bool init();

    /**
     * @brief Terminates memory connection - each with his own implementation
     */
    void terminate();

    /**
     * @param Address to read from
     * @param Number of bytes to read
     * @brief
     */
    void* read_bytes(const address_t address, const size_t length);

    /**
     * @param Starting m_address
     * @param List of offsets to dereference from base
     * @brief
     */
    address_t read_chain(const address_t base, const std::vector<address_t>& offsets);

    /**
     * @param Address of string
     * @brief Reads UTF-8 string from a target m_address
     */
    std::string read_narrow_string(const address_t address);

    /**
     * @param Address to read from
     */
    template<typename T>
    T read(address_t address) { // Add your own implementation
        return T();
    }

    template<typename T>
    std::vector<T> read_many(address_t address, std::size_t count) {
        if (count <= 0) {
            return std::vector<T>();
        }

        auto buffer = (T*)read_bytes(address, count * sizeof(T));
        if (buffer == nullptr) {
            return std::vector<T>();
        }

        std::vector<T> ret;
        for (auto index = 0; index < count; ++index) {
            ret.push_back(buffer[index]);
        }

        free(buffer);
        return ret;
    }

    /**
     * @param Address to write to
     * @param Data to write
     */
    template<typename T>
    void write(address_t address, T data) { // Add your own implementation
        return;
    }
}

#endif //MEMORY_H
