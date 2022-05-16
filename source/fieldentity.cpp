#include "fieldentity.h"

FieldEntity::FieldEntity(int id,QString name,int type,int length,int isPK,int isNull,int isUnique,QString comment):
    m_id(id),
    m_sName(name),
    m_iType(type),
    m_iLength(length),
    m_isPK(isPK),
    m_isNull(isNull),
    m_isUnique(isUnique),
    m_sComment(comment)
{
    this->m_iOrder = 0;
    this->m_dMax = INT_MIN;
    this->m_dMin = INT_MAX;
    this->m_sDefault = QString("");
}

FieldEntity::~FieldEntity(void)
{
}
