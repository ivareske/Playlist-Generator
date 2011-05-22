#include "Rule.h"


Rule::Rule(){
    
    type_ = UNKNOWN;
    shouldBeTrue_ = true;
    caseSensitive_ = true;
}


Rule::ruleType Rule::type() const{
    return type_;
}

QString Rule::value() const{
    return value_;
}

bool Rule::shouldBeTrue() const{
    return shouldBeTrue_;
}

bool Rule::caseSensitive() const{
    return caseSensitive_;
}


void Rule::setType( ruleType type ){
    type_ = type;
}

void Rule::setValue( const QString &value ){
    value_ = value;
}

void Rule::setShouldBeTrue( bool shouldBeTrue ){
    shouldBeTrue_ = shouldBeTrue;
}

void Rule::setCaseSensitive( bool caseSensitive ){
    caseSensitive_ = caseSensitive;
}

Rule::operator QVariant () const{
    return QVariant::fromValue(*this);
}

QDataStream &operator>>(QDataStream &in, Rule &r){
    quint32 type;
    QString value;
    bool shouldBeTrue;
    bool caseSensitive;
    in >> type >> value >> shouldBeTrue >> caseSensitive;
    r = Rule();
    r.setType(static_cast<Rule::ruleType>(type)); r.setValue(value); r.setCaseSensitive(caseSensitive); r.setShouldBeTrue(shouldBeTrue);
    return in;
}

QDataStream &operator<<(QDataStream &out, const Rule &r){
    out << static_cast<quint32>(r.type()) << r.value() << r.caseSensitive() << r.shouldBeTrue();
    return out;
}



