//
// Created by Akira Mujawar on 06/07/22.
//

#ifndef VULKAN_ENGINE_INTERNAL_PTR_HPP
#define VULKAN_ENGINE_INTERNAL_PTR_HPP

#include "memory"

// Using Pimpl Pattern to reduce build times
namespace MeowEngine {
    namespace internal_ptr_deleter {
        // 1. Custom deleter
        template <class T>

        void deleter(T* victim) {
            delete victim;
        }
    } // namespace internal_ptr_deleter

    // 2. Template definition, alias of std::unique_ptr
    template <class T, class Deleter = void (*)(T*)>
    using internal_ptr = std::unique_ptr<T, Deleter>;

    // 3. Factory to create internal_ptr instance
    template <class T, class... Args>
    inline internal_ptr<T> make_internal_ptr(Args&&... args) {
        return internal_ptr<T>(new T(std::forward<Args>(args)...), &internal_ptr_deleter::deleter<T>);
    }

} // namespace MeowEngine

#endif //VULKAN_ENGINE_INTERNAL_PTR_HPP
