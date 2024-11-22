#pragma once

#include <foxogram/export.h>
#include <exception>
#include <string>
#include <ixwebsocket/IXHttpClient.h>

#define decl_exception(name, Msg, ErrCode, HttpCode) class name : public foxogram::Exception { \
    protected: \
        std::string msg = Msg; \
        int errCode = ErrCode; \
        int httpCode = HttpCode; \
    public: \
        using foxogram::Exception::Exception; \
        explicit name(const std::string& what, int& errCode, int& httpCode) : msg(what), errCode(errCode), \
        httpCode(httpCode) {} \
        explicit name(std::string&& what, int&& errCode, int&& httpCode) : msg(std::move(what)), \
        errCode(errCode), httpCode(httpCode) {} \
        name(const name&) = default; \
        name(name&&) = default; \
        ~name() override = default; \
        const char* what() const noexcept override { return msg.c_str(); }; \
        int errorCode() const noexcept {return errCode; }; \
        int httpErrorCode() const noexcept {return httpCode; }; \
};

namespace foxogram {
    class FOXOGRAM_LIB_EXPORT Exception : public std::exception {
    protected:
        std::string msg;
        int errCode;
        int httpCode;
    public:
        using std::exception::exception;
        explicit Exception(const std::string& what, int& errCode, int& httpCode) : msg(what), errCode(errCode),
        httpCode(httpCode) {}
        explicit Exception(std::string&& what, int&& errCode, int&& httpCode) : msg(std::move(what)),
        errCode(errCode), httpCode(httpCode) {}
        Exception(const Exception&) = default;
        Exception(Exception&&) = default;
        ~Exception() override = default;
        const char* what() const noexcept override { return msg.c_str(); };
        int errorCode() const noexcept {return errCode; };
        int httpErrorCode() const noexcept {return httpCode; };
    };

    decl_exception(MessageNotFoundException, std::string("Unable to find message(s) for this channel or matching these parameters"), 101, 404)
    decl_exception(ChannelNotFoundException, std::string("Unknown channel"), 201, 404)
    decl_exception(UserEmailNotVerfiedException, std::string("You need to verify your email first"), 302, 403)
    decl_exception(UserAuthenticationNeededException, std::string("You need to authenticate to perform this action"), 303, 403)
    decl_exception(UserAlreadyExistsException, std::string("User with this email already exist"), 304, 403)
    decl_exception(UserUnauthorizatedException, std::string("You need to authorize first."), 301, 401)
    decl_exception(InvalidCredentialsException, std::string("Invalid password or email"), 305, 400)
    decl_exception(InvalidCodeException, std::string("Code is invalid!"), 401, 404)
    decl_exception(CodeExpiredException, std::string("Code has expired!"), 401, 404)
    decl_exception(MemberInChannelNotFoundException, std::string("Can't find member in this channel"), 401, 404)
    decl_exception(MissingPermissionException, std::string("You don't have enough permissions to delete this channel"), 402, 404)

}