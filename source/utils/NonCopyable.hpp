#ifndef RAVENENGINE_NONCOPYABLE_HPP
#define RAVENENGINE_NONCOPYABLE_HPP


namespace re {

    /**
     * @brief Pure virtual class with default constructors for not allow copy operations
     */
    class NonCopyable {
    protected:
        NonCopyable() = default;

        virtual ~NonCopyable() = default;

    public:
        NonCopyable(const NonCopyable &) = delete;

        NonCopyable(NonCopyable &&) = default;

        NonCopyable &operator=(const NonCopyable &) = delete;

        NonCopyable &operator=(NonCopyable &&) = default;
    };

} // namespace re


#endif //RAVENENGINE_NONCOPYABLE_HPP
