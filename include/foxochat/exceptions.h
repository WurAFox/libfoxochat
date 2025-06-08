#pragma once

#include <foxochat/export.h>
#include <exception>
#include <string>
#include <iostream>

#define decl_exception(name, Msg, Exception) class name : public foxochat::Exception { \
    protected: \
        std::string msg = Msg; \
    public: \
        using foxochat::Exception::Exception; \
        explicit name(const std::string& what) : msg(what) {std::cout << what << std::endl;} \
        explicit name(std::string&& what) : msg(std::move(what)) {std::cout << what << std::endl;} \
        name(const name&) = default; \
        name(name&&) = default; \
        name() = default; \
        ~name() override = default; \
        const char* what() const noexcept override { return msg.c_str(); }; \
};

namespace foxochat {
    class LIBFOXOCHAT_EXPORT Exception : public std::exception {
    protected:
        std::string msg;
    public:
        using std::exception::exception;
        explicit Exception(const std::string& what) : msg(what) {}
        explicit Exception(std::string&& what) : msg(std::move(what)) {}
        Exception(const Exception&) = default;
        Exception(Exception&&) = default;
        ~Exception() override = default;
        [[nodiscard]] const char* what() const noexcept override { return msg.c_str(); };
    };

    decl_exception(WebSocketException   , std::string(""), Exception)
    decl_exception(HttpException, std::string(""), Exception)
    decl_exception(MessageNotFoundException, std::string("Unable to find message(s) for this channel or matching these parameters"), HttpException)
    decl_exception(ChannelNotFoundException, std::string("Unknown channel"), HttpException)
    decl_exception(UserEmailNotVerfiedException, std::string("You need to verify your email first"), HttpException)
    decl_exception(UserWithThisEmailAlreadyExistException, std::string("User with this email already exist"), HttpException)
    decl_exception(UserUnauthorizatedException, std::string("You need to authorize first."), HttpException)
    decl_exception(UserCredentialsIsInvalidException, std::string("Invalid password or email"), HttpException)
    decl_exception(CodeIsInvalidException, std::string("Code is invalid!"), HttpException)
    decl_exception(CodeExpiredException, std::string("Code has expired!"), HttpException)
    decl_exception(MemberInChannelNotFoundException, std::string("Can't find member in this channel"), HttpException)
    decl_exception(MissingPermissionException, std::string("You don't have enough permissions to delete this channel"), HttpException)
    decl_exception(MemberAlreadyInChannelException, std::string("You've already joined this channel."), HttpException)

}