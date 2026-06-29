
#line 41 "../../../../qt-everywhere-src-6.10.2/qtremoteobjects/src/repparser/parser.g"

#ifndef REPPARSER_H
#define REPPARSER_H

#include <rep_grammar_p.h>
#include <qregexparser.h>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QRegularExpression>
#include <QSet>

QT_BEGIN_NAMESPACE
class QIODevice;
class QCryptographicHash;

struct AST;

struct SignedType
{
    SignedType(const QString &name = QString());
    virtual ~SignedType() {}
    void generateSignature(AST &ast);
    virtual QString typeName() const;
    virtual void signature_impl(const AST &ast, QCryptographicHash &checksum) = 0;
    QString name;
    QString compilerAttribute;
};

/// A property of a Class declaration
struct ASTProperty
{
    enum Modifier
    {
        Constant,
        ReadOnly,
        ReadPush,
        ReadWrite,
        SourceOnlySetter
    };

    ASTProperty();
    ASTProperty(const QString &type, const QString &name, const QString &defaultValue, Modifier modifier, bool persisted,
                bool isPointer=false);

    QString type;
    QString name;
    QString defaultValue;
    Modifier modifier;
    bool persisted;
    bool isPointer;
};
Q_DECLARE_TYPEINFO(ASTProperty, Q_RELOCATABLE_TYPE);

struct ASTDeclaration
{
    enum VariableType {
        None = 0,
        Constant = 1,
        Reference = 2,
    };
    Q_DECLARE_FLAGS(VariableTypes, VariableType)

    ASTDeclaration(const QString &declarationType = QString(), const QString &declarationName = QString(), VariableTypes declarationVariableType = None)
        : type(declarationType),
          name(declarationName),
          variableType(declarationVariableType)
    {
    }

    QString asString(bool withName) const;

    QString type;
    QString name;
    VariableTypes variableType;
};
Q_DECLARE_TYPEINFO(ASTDeclaration, Q_RELOCATABLE_TYPE);

struct ASTFunction
{
    enum ParamsAsStringFormat {
        Default,
        Normalized
    };

    explicit ASTFunction(const QString &name = QString(), const QString &returnType = QLatin1String("void"));

    QString paramsAsString(ParamsAsStringFormat format = Default) const;
    QStringList paramNames() const;

    QString returnType;
    QString name;
    QList<ASTDeclaration> params;
};
Q_DECLARE_TYPEINFO(ASTFunction, Q_RELOCATABLE_TYPE);

struct ASTEnumParam
{
    ASTEnumParam(const QString &paramName = QString(), int paramValue = 0)
        : name(paramName),
          value(paramValue)
    {
    }

    QString name;
    int value;
};
Q_DECLARE_TYPEINFO(ASTEnumParam, Q_RELOCATABLE_TYPE);

struct ASTEnum : public SignedType
{
    explicit ASTEnum(const QString &name = QString());
    void signature_impl(const AST &ast, QCryptographicHash &checksum) override;
    QString typeName() const override;

    QString type;
    QString scope;
    QList<ASTEnumParam> params;
    bool isSigned;
    bool isScoped;
    int max;
    int flagIndex = -1;
};
Q_DECLARE_TYPEINFO(ASTEnum, Q_RELOCATABLE_TYPE);

struct ASTFlag : public SignedType
{
    explicit ASTFlag(const QString &name = {}, const QString &_enum = {});
    void signature_impl(const AST &ast, QCryptographicHash &checksum) override;
    QString typeName() const override;

    bool isValid() const;
    QString _enum;
    QString scope;
};
Q_DECLARE_TYPEINFO(ASTFlag, Q_RELOCATABLE_TYPE);

struct ASTModelRole
{
    ASTModelRole(const QString &roleName = QString())
        : name(roleName)
    {
    }

    QString name;
};
Q_DECLARE_TYPEINFO(ASTModelRole, Q_RELOCATABLE_TYPE);

struct ASTModel : public SignedType
{
    ASTModel(const QString &name, const QString &scope, int index = -1)
        : SignedType(name), scope(scope), propertyIndex(index) {}
    void signature_impl(const AST &ast, QCryptographicHash &checksum) override;
    QString typeName() const override;

    QList<ASTModelRole> roles;
    QString scope;
    int propertyIndex;
};
Q_DECLARE_TYPEINFO(ASTModel, Q_RELOCATABLE_TYPE);

/// A Class declaration
struct ASTClass : public SignedType
{
    explicit ASTClass(const QString& name = QString());
    void signature_impl(const AST &ast, QCryptographicHash &checksum) override;

    bool isValid() const;
    bool hasPointerObjects() const;

    QList<ASTProperty> properties;
    QList<ASTFunction> signalsList;
    QList<ASTFunction> slotsList;
    QList<ASTEnum> enums;
    QList<ASTFlag> flags;
    bool hasPersisted;
    QList<ASTModel> modelMetadata;
    QList<int> subClassPropertyIndices;
};
Q_DECLARE_TYPEINFO(ASTClass, Q_RELOCATABLE_TYPE);

// The attribute of a POD
struct PODAttribute
{
    explicit PODAttribute(const QString &type_ = QString(), const QString &name_ = QString())
        : type(type_),
          name(name_)
    {}
    QString type;
    QString name;
};
Q_DECLARE_TYPEINFO(PODAttribute, Q_RELOCATABLE_TYPE);

// A POD declaration
struct POD : public SignedType
{
    void signature_impl(const AST &ast, QCryptographicHash &checksum) override;

    QList<PODAttribute> attributes;
    QList<ASTEnum> enums;
    QList<ASTFlag> flags;
};
Q_DECLARE_TYPEINFO(POD, Q_RELOCATABLE_TYPE);

// The AST representation of a .rep file
struct AST
{
    QList<ASTClass> classes;
    QList<POD> pods;
    QList<ASTEnum> enums;
    QList<ASTFlag> flags;
    QList<QString> enumUses;
    QStringList preprocessorDirectives;
    QStringList headerLines;
    QStringList footerLines;
    QHash<QString, QByteArray> typeSignatures;
    QByteArray typeData(const QString &type, const QString &className) const;
    QByteArray functionsData(const QList<ASTFunction> &functions, const QString &className) const;
};
Q_DECLARE_TYPEINFO(AST, Q_RELOCATABLE_TYPE);

class RepParser: public QRegexParser<RepParser, rep_grammar>
{
public:
    explicit RepParser(QIODevice &outputDevice);
    virtual ~RepParser() {}

    bool parse() override { return QRegexParser<RepParser, rep_grammar>::parse(); }

    void reset() override;
    int nextToken();
    bool consumeRule(int ruleno);

    AST ast() const;

private:
    struct TypeParser
    {
        void parseArguments(const QString &arguments);
        void appendParams(ASTFunction &slot);
        void appendPods(POD &pods);
        void generateFunctionParameter(QString variableName, const QString &propertyType, int &variableNameIndex, ASTDeclaration::VariableTypes variableType);
        //Type, Variable
        QList<ASTDeclaration> arguments;
    };

    bool parseProperty(ASTClass &astClass, const QString &propertyDeclaration);
    /// A helper function to parse modifier flag of property declaration
    bool parseModifierFlag(const QString &flag, ASTProperty::Modifier &modifier, bool &persisted);

    bool parseRoles(ASTModel &astModel, const QString &modelRoles);

    AST m_ast;

    ASTClass m_astClass;
    POD m_astPod;
    QString m_symbol;
    QString m_argString;
    ASTEnum m_astEnum;
    int m_astEnumValue;
};
QT_END_NAMESPACE
#endif
