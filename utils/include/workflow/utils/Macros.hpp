#pragma once

#define SEQ_INTERFACE_DECL( classname ) \
    public: \
        virtual ~classname() = 0; \
        classname(); \
        classname( const classname& other ) = delete; \
        classname& operator=( const classname& other ) = delete

#define SEQ_INTERFACE_IMPL( classname ) \
    classname::~classname() = default;  \
    classname::classname() = default

#define SEQ_POINTER_DECL( classname ) \
    class classname; \
    using classname ## SharedPtr = std::shared_ptr<classname>; \
    using classname ## UniquePtr = std::unique_ptr<classname>; \
    using classname ## WeakPtr = std::weak_ptr<classname>
