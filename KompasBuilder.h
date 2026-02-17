#pragma once

#include <vector>

// Нужны структуры параметров, которые уже рассчитаны и показаны в editbox-ах.
#include "KURSACHDoc.h"

// --- KOMPAS-3D API5 ---
#include "C:\\Program Files\\ASCON\\KOMPAS-3D v23 Study\\SDK\\Include\\ksConstants.h"
#include "C:\\Program Files\\ASCON\\KOMPAS-3D v23 Study\\SDK\\Include\\ksConstants3D.h"

#import "C:\\Program Files\\ASCON\\KOMPAS-3D v23 Study\\SDK\\lib\\kAPI5.tlb" \
    rename("GetObject", "KompasGetObject") \
    rename("min",       "KompasMin") \
    rename("max",       "KompasMax")

#include <comdef.h>

// Вынесено из txt «Функции построения…».
// ВАЖНО: здесь нет «захардкоженных» размеров из примера – размеры берутся из
// параметров, приходящих из диалогов (editbox-ов) через CKURSACHDoc.

class CKompasBuilder
{
public:
    explicit CKompasBuilder(Kompas6API5::KompasObjectPtr app);

    // halfCouplingData: вектор из CKURSACHDoc::GetHalfCouplingData (индексы как в CObjectsDlg).
    void CreatePoluMufta(const std::vector<double>& halfCouplingData);
    void CreateBoltGOST7817(const std::vector<double>& halfCouplingData, const CKURSACHDoc::BoltUIParams& ui);
    void CreateBoltGOST7796(const std::vector<double>& halfCouplingData, const CKURSACHDoc::BoltUIParams& ui);
    void CreateGaykaGOST15521(const std::vector<double>& halfCouplingData, const CKURSACHDoc::NutUIParams& ui);
    void CreateShaybaGOST6402(const std::vector<double>& halfCouplingData);
    void CreateSborka();

private:
    Kompas6API5::KompasObjectPtr m_app;
    Kompas6API5::ksDocument3DPtr m_doc;
    Kompas6API5::ksPartPtr m_part;

    // Утилита: безопасно получить double из вектора.
    static double GetD(const std::vector<double>& v, size_t idx, double def = 0.0);
    static int GetI(const std::vector<double>& v, size_t idx, int def = 0);

    static int NominalFromHoleD2(double d2_hole);

    static double SFromNominal(int nominal);

    static double NutMFromNominal(int nominal);
};
