#pragma once

#include <iostream>
#include <string>
#include <stack>

namespace tt
{
    namespace util
    {
        namespace debug
        {
            struct block { std::string name; };

            struct end_block {};

            struct end_row {};

            struct dout
            {
            private:
                inline static dout *inst_{nullptr};
                
                std::ostream *os_{nullptr};
                bool nline_ = true;
                std::stack<block> blocks_{};

                dout(std::ostream *os) : os_(os) {}

                void prep() {
                    for(int i = 0; i < blocks_.size(); i++)
                        (*os_) << "  ";
                }

            public:
                static dout &init(std::ostream *os)
                {
                    if (inst_ != nullptr)
                        delete inst_;
                    inst_ = new dout(os);
                    return *inst_;
                }

                static dout &instance()
                {
                    if(inst_ == nullptr)
                        init(nullptr);
                    return *inst_;
                }

                static bool is_inited()
                {
                    return inst_ != nullptr;
                }

                dout& operator<<(end_row&& a) {
                    if(os_ == nullptr)
                        return *this;
                    if(nline_)
                        return *this;
                    (*os_) << '\n';
                    prep();
                    nline_ = true;
                    return *this;
                }

                template <typename T>
                dout &operator<<(T &&obj)
                {
                    if(os_ == nullptr)
                        return *this;
                    (*os_) << obj;
                    nline_ = false;
                    return *this;
                }

                dout& operator<<(block&& a) {
                    (*this) << end_row{} << a.name;
                    blocks_.push(a);
                    (*this) << end_row{};
                    return *this;
                }

                dout& operator<<(end_block&& a) {
                    auto r = blocks_.top().name;
                    blocks_.pop();
                    (*this) << end_row{} << "end " << r << end_row{};
                    return *this;
                }
            };
        }
    }
}