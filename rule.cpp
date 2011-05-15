#include "rule.h"


rule::rule(){
    
    type_ = UNKNOWN;
    shouldBeTrue_ = true;
    caseSensitive_ = true;
}


rule::ruleType rule::type() const{
    return type_;
}

QString rule::value() const{
    return value_;
}

bool rule::shouldBeTrue() const{
    return shouldBeTrue_;
}

bool rule::caseSensitive() const{
    return caseSensitive_;
}


void rule::setType( ruleType type ){
    type_ = type;
}

void rule::setValue( const QString &value ){
    value_ = value;
}

void rule::setShouldBeTrue( bool shouldBeTrue ){
    shouldBeTrue_ = shouldBeTrue;
}

void rule::setCaseSensitive( bool caseSensitive ){
    caseSensitive_ = caseSensitive;
}

rule::operator QVariant () const{
    return QVariant::fromValue(*this);
}

QDataStream &operator>>(QDataStream &in, rule &r){
    quint32 type;
    QString value;
    bool shouldBeTrue;
    bool caseSensitive;
    in >> type >> value >> shouldBeTrue >> caseSensitive;
    r = rule();
    r.setType(static_cast<rule::ruleType>(type)); r.setValue(value); r.setCaseSensitive(caseSensitive); r.setShouldBeTrue(shouldBeTrue);
    return in;
}

QDataStream &operator<<(QDataStream &out, const rule &r){
    out << static_cast<quint32>(r.type()) << r.value() << r.caseSensitive() << r.shouldBeTrue();
    return out;
}



