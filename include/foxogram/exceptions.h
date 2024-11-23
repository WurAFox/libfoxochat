#pragma once

#include <foxogram/export.h>
#include <exception>
#include <string>
#include <ixwebsocket/IXHttpClient.h>

#define decl_exception(name, Msg, Exception) class name : public foxogram::Exception { \
    protected: \
        std::string msg = Msg; \
    public: \
        using foxogram::Exception::Exception; \
        explicit name(const std::string& what) = delete; \
        explicit name(std::string&& what) = delete; \
        name(const name&) = delete; \
        name(name&&) = delete; \
        name() = default; \
        ~name() override = default; \
        const char* what() const noexcept override { return msg.c_str(); }; \
};

namespace foxogram {
    class FOXOGRAM_LIB_EXPORT HttpException : public std::exception {
    protected:
        std::string msg;
    public:
        using std::exception::exception;
        explicit HttpException(const std::string& what) : msg(what) {}
        explicit HttpException(std::string&& what) : msg(std::move(what)) {}
        HttpException(const HttpException&) = default;
        HttpException(HttpException&&) = default;
        ~HttpException() override = default;
        const char* what() const noexcept override { return msg.c_str(); };
    };

    decl_exception(MessageNotFoundException, std::string("Unable to find message(s) for this channel or matching these parameters"), HttpException)
    decl_exception(ChannelNotFoundException, std::string("Unknown channel"), HttpException)
    decl_exception(UserEmailNotVerfiedException, std::string("You need to verify your email first"), HttpException)
    decl_exception(UserAuthenticationNeededException, std::string("You need to authenticate to perform this action"), HttpException)
    decl_exception(UserWithThisEmailAlreadyExistException, std::string("User with this email already exist"), HttpException)
    decl_exception(UserUnauthorizatedException, std::string("You need to authorize first."), HttpException)
    decl_exception(UserCredentialsIsInvalidException, std::string("Invalid password or email"), HttpException)
    decl_exception(CodeIsInvalidException, std::string("Code is invalid!"), HttpException)
    decl_exception(CodeExpiredException, std::string("Code has expired!"), HttpException)
    decl_exception(MemberInChannelNotFoundException, std::string("Can't find member in this channel"), HttpException)
    decl_exception(MissingPermissionException, std::string("You don't have enough permissions to delete this channel"), HttpException)
    decl_exception(MemberAlreadyInChannelException, std::string("You've already joined this channel."), HttpException)

}