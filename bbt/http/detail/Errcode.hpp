#pragma once
#include <string>
#include <bbt/base/errcode/Errcode.hpp>
#include <optional>

namespace bbt::http
{

class Errcode;

enum emErr
{
    Failed = 0,
    BadParams = 1,
};

typedef std::optional<Errcode> ErrOpt;

class Errcode:
    public bbt::errcode::Errcode<emErr>
{
public:
    typedef bbt::errcode::Errcode<emErr> TBase;

    explicit Errcode(const std::string& msg, emErr err_type = emErr::Failed):TBase(msg, err_type) {}
    virtual ~Errcode() {}

    virtual const std::string&  What() const override  { return GetMsg(); }
    virtual const char*         CWhat() const override { return What().c_str(); }
    virtual const emErr&        Type() const override  { return GetErrType(); }
};

} // namespace bbt::http
