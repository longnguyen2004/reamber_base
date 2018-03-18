#ifndef COM_TPLIST_H
#define COM_TPLIST_H


#include "com_tp.h"


class  cOM_TPList
{
public:
    cOM_TPList();
    cOM_TPList(QList<cOM_TP> newOM_TPList); // Initialize via

    cOM_TP   operator [](int i) const;
    cOM_TP & operator [](int i);

    QList<double> getOffsetList();
    QList<double> getCodeList(int onlyFlag = SV_BPM_ONLY);
    QList<double> getValueList(int onlyFlag = SV_ONLY);

    double getMinOffset();
    double getMaxOffset();
    double getLength   ();
    int    getSize     ();

    double getAverageSV ();
    double getAverageBPM();

    void append     (cOM_TP newOM_TP) { OM_TPList.append(newOM_TP); }
    void deleteIndex(unsigned  index) { OM_TPList.removeAt(index); }

    const static int SV_BPM_ONLY  = 0,
                     SV_ONLY      = 1,
                     BPM_ONLY     = 2;

protected:
    QList<cOM_TP> OM_TPList;
};

#endif // COM_TPLIST_H
