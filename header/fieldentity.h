#ifndef FIELDENTITY_H
#define FIELDENTITY_H
#include <QString>
#define DB_INTEGER 1
#define DEFAULT_FIELD_LENGTH 20
#define INVALID	0
#define YES 1
#define NO 0

class FieldEntity
{
private:
    int m_id;
    QString m_sName;
    int m_iOrder;
    int m_iType;
    int m_iLength;
    double m_dMax;
    double m_dMin;
    QString m_sDefault;
    int m_isPK;
    int m_isNull;
    int m_isUnique;
    QString m_sComment;

public:
    //FieldEntity(int id=INVALID,QString name="NewField",int type=DB_INTEGER,int length=DEFAULT_FIELD_LENGTH,
     //               int isPk=NO,int isNull=YES,int isUnique=NO,QString comment="");
    FieldEntity(int id,QString name,int type,int length,int isPK,int isNull,int isUnique,QString comment);
    //FieldEntity(QString &str);
    ~FieldEntity(void);

    int GetId() { return m_id; };
    QString GetName() { return m_sName; };
    int GetOrder() { return m_iOrder; };
    int GetType() { return m_iType; };
    int GetLength() { return m_iLength; };
    double GetMax() { return m_dMax; };
    double GetMin() { return m_dMin; };
    QString GetDefault() { return m_sDefault; };
    int GetIsPK() { return m_isPK; };
    int GetIsNull() { return m_isNull; };
    int GetIsUnique() { return m_isUnique; };
    QString GetComment() { return m_sComment; };

    void SetId(int id) { m_id=id; };
    void SetName(QString name) { m_sName=name; };
    void SetOrder(int order) { m_iOrder=order; };
    void SetType(int type) { m_iType=type; };
    void SetLength(int length) { m_iLength=length; };
    void SetMax(double max) { m_dMax=max; };
    void SetMin(double min) { m_dMin=min; };
    void SetDefault(QString dft) { m_sDefault=dft; };
    void SetIsPK(int isPK) { m_isPK=isPK; };
    void SetIsNull(int isNull) { m_isNull=isNull; };
    void SetIsUnique(int isUnique) { m_isUnique=isUnique; };
    void SetComment(QString comment) { m_sComment=comment; };

};

#endif // FIELDENTITY_H
