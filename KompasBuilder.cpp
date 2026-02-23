#include "pch.h"

#include "KompasBuilder.h"

#include <cmath>

using namespace Kompas6API5;

static double RoundToNearestInt(double v)
{
    return std::floor(v + 0.5);
}

static int NominalFromHole(double hole)
{
    // По вашему проекту: отверстия под 7817: 8.4/10.5; под 7796: 9/11.
    // Приводим к номиналу резьбы M8/M10/M12.
    if (hole <= 8.6) return 8;
    if (hole <= 10.7) return 10;
    return 12;
}

static double NutHeightM(int nominal)
{
    switch (nominal)
    {
    case 8:  return 4.0;
    case 10: return 5.0;
    case 12: return 6.0;
    default: return 5.0;
    }
}

static double WrenchS(int nominal)
{
    // Размер под ключ
    switch (nominal)
    {
    case 8:  return 12.0;
    case 10: return 14.0;
    case 12: return 17.0;
    default: return 14.0;
    }
}

double CKompasBuilder::GetD(const std::vector<double>& v, size_t idx, double def)
{
    return (idx < v.size()) ? v[idx] : def;
}

int CKompasBuilder::GetI(const std::vector<double>& v, size_t idx, int def)
{
    return (idx < v.size()) ? (int)RoundToNearestInt(v[idx]) : def;
}

CKompasBuilder::CKompasBuilder(KompasObjectPtr app)
    : m_app(app)
{
}

void CKompasBuilder::CreatePoluMufta(const std::vector<double>& dh)
{
    m_doc = m_app->Document3D();
    m_doc->Create(false, true);
    m_part = m_doc->GetPart(pTop_Part);

    // Параметры
    const double l = GetD(dh, 18, 42.0);
    const double h = GetD(dh, 4, 40.0);
    const double B = GetD(dh, 3, 14.0);
    const double d0 = GetD(dh, 12, 18.0);
    const double l1 = GetD(dh, 19, 8.0);
    const double D = GetD(dh, 10, 80.0);
    const double d = GetD(dh, 1, 11.0);
    const double d3 = GetD(dh, 14, 8.4);
    const double d4 = GetD(dh, 16, 9.0);
    const double D1 = GetD(dh, 11, 55.0);
    double M = GetD(dh, 0, 16.0);
    const double R = GetD(dh, 21, 0.2);

    // Эскиз №1 полумуфты 
    ksEntityPtr pSketch = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
    pSketchDef->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketch->Create();
    ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();
    p2DDoc->ksLineSeg(0, 0, l, 0, 1);
    p2DDoc->ksLineSeg(l, 0, l, d0 / 2, 1);
    p2DDoc->ksLineSeg(l, d0 / 2, l1, d0 / 2, 1);
    p2DDoc->ksLineSeg(l1, d0 / 2, l1, D / 2, 1);
    p2DDoc->ksLineSeg(l1, D / 2, 0, D / 2, 1);
    p2DDoc->ksLineSeg(0, D / 2, 0, 0, 1);
    p2DDoc->ksLineSeg(-10, 0, 10, 0, 3);
    pSketchDef->EndEdit();

    // Выдавливание вращением 
    ksEntityPtr pRotate = m_part->NewEntity(o3d_bossRotated);
    ksBossRotatedDefinitionPtr pRotDef = pRotate->GetDefinition();
    pRotDef->SetSketch(pSketch);
    pRotDef->SetSideParam(TRUE, 360);
    pRotate->Create();

    // ПЛОСКОСТЬ ДЛЯ ОТВЕРСТИЙ
    ksEntityCollectionPtr faces = m_part->EntityCollection(o3d_face);
    ksEntityPtr endFace = nullptr;
    for (int i = 0; i < faces->GetCount(); i++)
    {
        ksEntityPtr face = faces->GetByIndex(i);
        ksFaceDefinitionPtr def = face->GetDefinition();
        if (def->IsPlanar()) { endFace = face; break; }
    }

    ksEntityPtr pPlane = m_part->NewEntity(o3d_planeOffset);
    ksPlaneOffsetDefinitionPtr pPlaneDef = pPlane->GetDefinition();
    pPlaneDef->direction = true;
    pPlaneDef->offset = l - l1;
    pPlaneDef->SetPlane(endFace);
    pPlane->Create();

    // ОТВЕРСТИЯ
    // Центральное
    ksEntityPtr pSketchHole1 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchHoleDef1 = pSketchHole1->GetDefinition();
    pSketchHoleDef1->SetPlane(pPlane);
    pSketchHole1->Create();
    ksDocument2DPtr p2DDocHole1 = pSketchHoleDef1->BeginEdit();
    p2DDocHole1->ksCircle(0, 0, d / 2, 1);
    p2DDocHole1->ksLineSeg(-10, 0, 10, 0, 3);
    pSketchHoleDef1->EndEdit();

    ksEntityPtr pCut1 = m_part->NewEntity(o3d_cutExtrusion);
    ksCutExtrusionDefinitionPtr pCutDef1 = pCut1->GetDefinition();
    pCutDef1->directionType = dtNormal;
    pCutDef1->SetSketch(pSketchHole1);
    pCutDef1->SetSideParam(true, etThroughAll, 0, 0, false);
    pCut1->Create();

    // ========== ФАСКИ ТОРЦА И НАРУЖНЫЕ (без отверстий) ==========
    m_part->Update(); // Обновляем для фасок

    // ФАСКА ТОРЦА X≈l
    ksEntityPtr pChamfer = m_part->NewEntity(o3d_chamfer);
    ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
    pChamferDef->SetChamferParam(true, 1, 1);
    ksEntityCollectionPtr fl = pChamferDef->array();
    fl->Clear();

    // НОВЫЕ edges после всех отверстий
    ksEntityCollectionPtr edges = m_part->EntityCollection(o3d_edge);

    for (int i = 0; i < edges->GetCount(); i++)
    {
        ksEntityPtr edge = edges->GetByIndex(i);
        if (!edge) continue;
        ksEdgeDefinitionPtr def = edge->GetDefinition();
        if (!def) continue;

        ksVertexDefinitionPtr v1 = def->GetVertex(true);
        if (!v1) continue;

        double x1, y1, z1;
        v1->GetPoint(&x1, &y1, &z1);
        double r1 = sqrt(y1 * y1 + z1 * z1);

        // ФАСКА ТОРЦА X≈l
        if (fabs(x1 - l) < 1.0)
            fl->Add(edge);
    }
    pChamfer->Create();

    // ОСТАЛЬНЫЕ ФАСКИ/СКРУГЛЕНИЯ
    ksEntityPtr pChamferOuter = m_part->NewEntity(o3d_chamfer);
    ksChamferDefinitionPtr pChamferOuterDef = pChamferOuter->GetDefinition();
    pChamferOuterDef->SetChamferParam(true, 1, 1);
    ksEntityCollectionPtr flOuter = pChamferOuterDef->array();
    flOuter->Clear();

    double outerR = D / 2.0;
    for (int i = 0; i < edges->GetCount(); i++)
    {
        ksEntityPtr edge = edges->GetByIndex(i);
        ksEdgeDefinitionPtr def = edge->GetDefinition();
        if (!def) continue;
        ksVertexDefinitionPtr v1 = def->GetVertex(true);
        if (!v1) continue;
        double x1, y1, z1;
        v1->GetPoint(&x1, &y1, &z1);
        double rad = sqrt(y1 * y1 + z1 * z1);
        if ((fabs(x1) < 1.0 || fabs(x1 - l1) < 1.0) && fabs(rad - outerR) < 1.0)
            flOuter->Add(edge);
    }
    pChamferOuter->Create();

    ksEntityPtr pFillet2 = m_part->NewEntity(o3d_fillet);
    ksFilletDefinitionPtr pFilletDef2 = pFillet2->GetDefinition();
    pFilletDef2->radius = R;
    ksEntityCollectionPtr fl2 = pFilletDef2->array();
    fl2->Clear();
    for (int i = 0; i < edges->GetCount(); i++)
    {
        ksEntityPtr edge = edges->GetByIndex(i);
        ksEdgeDefinitionPtr def = edge->GetDefinition();
        if (!def) continue;
        ksVertexDefinitionPtr v1 = def->GetVertex(true);
        if (!v1) continue;
        double x1, y1, z1;
        v1->GetPoint(&x1, &y1, &z1);
        if (fabs(x1 - l1) < 2.0)
        {
            double radius = sqrt(y1 * y1 + z1 * z1);
            if (radius < outerR - 1.0)
                fl2->Add(edge);
        }
    }
    pFillet2->Create();

    // Болт 7817 №3 (d3=8.4мм)
    ksEntityPtr pSketchHole2 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchHoleDef2 = pSketchHole2->GetDefinition();
    pSketchHoleDef2->SetPlane(pPlane);
    pSketchHole2->Create();
    ksDocument2DPtr p2DDocHole2 = pSketchHoleDef2->BeginEdit();
    p2DDocHole2->ksCircle(0, -D1 / 2, d3 / 2, 1);
    p2DDocHole2->ksLineSeg(-10, 0, 10, 0, 3);
    pSketchHoleDef2->EndEdit();

    ksEntityPtr pCut2 = m_part->NewEntity(o3d_cutExtrusion);
    ksCutExtrusionDefinitionPtr pCutDef2 = pCut2->GetDefinition();
    pCutDef2->directionType = dtNormal;
    pCutDef2->SetSketch(pSketchHole2);
    pCutDef2->SetSideParam(true, etThroughAll, 0, 0, false);
    pCut2->Create();

    // Болт 7796 №4 (d4=9.0мм)
    ksEntityPtr pSketchHole3 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchHoleDef3 = pSketchHole3->GetDefinition();
    pSketchHoleDef3->SetPlane(pPlane);
    pSketchHole3->Create();
    ksDocument2DPtr p2DDocHole3 = pSketchHoleDef3->BeginEdit();
    p2DDocHole3->ksCircle(0, D1 / 2, d4 / 2, 1);
    p2DDocHole3->ksLineSeg(-10, 0, 10, 0, 3);
    pSketchHoleDef3->EndEdit();

    ksEntityPtr pCut3 = m_part->NewEntity(o3d_cutExtrusion);
    ksCutExtrusionDefinitionPtr pCutDef3 = pCut3->GetDefinition();
    pCutDef3->directionType = dtNormal;
    pCutDef3->SetSketch(pSketchHole3);
    pCutDef3->SetSideParam(true, etThroughAll, 0, 0, false);
    pCut3->Create();

    // ФАСКИ ОТВЕРСТИЙ ПО РАДИУСУ 27.5мм ТОЛЬКО ЛИЦЕВАЯ СТОРОНА
    m_part->Update();

    ksEntityPtr pChamferHole3 = m_part->NewEntity(o3d_chamfer);
    ksChamferDefinitionPtr def3 = pChamferHole3->GetDefinition();
    def3->SetChamferParam(true, 1, 1);
    ksEntityCollectionPtr edges3 = def3->array();

    ksEntityPtr pChamferHole4 = m_part->NewEntity(o3d_chamfer);
    ksChamferDefinitionPtr def4 = pChamferHole4->GetDefinition();
    def4->SetChamferParam(true, 1, 1);
    ksEntityCollectionPtr edges4 = def4->array();

    ksEntityCollectionPtr allEdges = m_part->EntityCollection(o3d_edge);

    for (int i = 0; i < allEdges->GetCount(); i++) {
        ksEntityPtr edge = allEdges->GetByIndex(i);
        ksEdgeDefinitionPtr edef = edge->GetDefinition();
        if (!edef) continue;

        ksVertexDefinitionPtr v1 = edef->GetVertex(true);
        ksVertexDefinitionPtr v2 = edef->GetVertex(false);

        if (v1) {
            double x1, y1, z1;
            v1->GetPoint(&x1, &y1, &z1);
            double dist1 = sqrt(y1 * y1 + z1 * z1);

            // ДОБАВЛЕНО: Только лицевая сторона X≈(l-l1)
            if (fabs(x1 - (l - l1)) < 1.0 && fabs(dist1 - 27.5) < 0.5) {
                if (fabs(y1) > fabs(z1)) edges3->Add(edge);
                else edges4->Add(edge);
            }
        }

        if (v2) {
            double x2, y2, z2;
            v2->GetPoint(&x2, &y2, &z2);
            double dist2 = sqrt(y2 * y2 + z2 * z2);

            if (fabs(x2 - (l - l1)) < 1.0 && fabs(dist2 - 27.5) < 0.5) {
                if (fabs(y2) > fabs(z2)) edges3->Add(edge);
                else edges4->Add(edge);
            }
        }
    }

    pChamferHole3->Create();
    pChamferHole4->Create();
    m_part->Update();


    // ========== МАРКЕРЫ (НОВЫЕ faces/edges) ==========
    m_part->Update();
    ksEntityCollectionPtr allFaces = m_part->EntityCollection(o3d_face);

    // Центральное отверстие
    for (int i = 0; i < allFaces->GetCount(); i++)
    {
        ksEntityPtr face = allFaces->GetByIndex(i);
        if (!face) continue;
        ksFaceDefinitionPtr def = face->GetDefinition();
        if (!def || !def->IsCylinder()) continue;
        double height, radius;
        def->GetCylinderParam(&height, &radius);
        if (fabs(radius - d / 2.0) < 0.1)
        {
            face->Putname(L"CentralHole");
            face->Update();
            break;
        }
    }

    m_part->Update();
    m_doc->SaveAs(L"C:\\\\Temp\\\\Полумуфта.m3d");
}

void CKompasBuilder::CreateGaykaGOST15521(const std::vector<double>& halfCouplingData, const CKURSACHDoc::NutUIParams& ui)
{
    m_doc = m_app->Document3D();
    m_doc->Create(false, true);
    m_part = m_doc->GetPart(pTop_Part);

    // Гайка: используем параметры из диалога (editbox-ов).
    // Если диалог не открывали, оставляем безопасный fallback на номинал по отверстиям.
    const double hole_for_7796 = GetD(halfCouplingData, 12, 9.0);
    const int nominal = (ui.nominal > 0) ? ui.nominal : NominalFromHole(hole_for_7796);

    const double S = (ui.S > 0) ? (double)ui.S : WrenchS(nominal);
    const double m = (ui.m > 0.0) ? ui.m : NutHeightM(nominal);
    const double d = (double)nominal;

    ksEntityPtr pSketch = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
    pSketchDef->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketch->Create();
    ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();

    const double R_out = S / std::sqrt(3.0);
    const double PI = 3.14159265358979323846;
    const double x1 = R_out * std::cos(PI / 6),  y1 = R_out * std::sin(PI / 6);
    const double x2 = R_out * std::cos(PI / 2),  y2 = R_out * std::sin(PI / 2);
    const double x3 = R_out * std::cos(5 * PI / 6), y3 = R_out * std::sin(5 * PI / 6);
    const double x4 = R_out * std::cos(7 * PI / 6), y4 = R_out * std::sin(7 * PI / 6);
    const double x5 = R_out * std::cos(3 * PI / 2), y5 = R_out * std::sin(3 * PI / 2);
    const double x6 = R_out * std::cos(11 * PI / 6), y6 = R_out * std::sin(11 * PI / 6);
    p2DDoc->ksLineSeg(x1, y1, x2, y2, 1);
    p2DDoc->ksLineSeg(x2, y2, x3, y3, 1);
    p2DDoc->ksLineSeg(x3, y3, x4, y4, 1);
    p2DDoc->ksLineSeg(x4, y4, x5, y5, 1);
    p2DDoc->ksLineSeg(x5, y5, x6, y6, 1);
    p2DDoc->ksLineSeg(x6, y6, x1, y1, 1);
    pSketchDef->EndEdit();

    ksEntityPtr pBoss = m_part->NewEntity(o3d_bossExtrusion);
    ksBossExtrusionDefinitionPtr pBossDef = pBoss->GetDefinition();
    pBossDef->SetSketch(pSketch);
    pBossDef->directionType = dtNormal;
    pBossDef->SetSideParam(true, etBlind, m, 0, false);
    pBoss->Create();

    // Отверстие
    ksEntityPtr pSketchHole = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchHoleDef = pSketchHole->GetDefinition();
    pSketchHoleDef->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketchHole->Create();
    p2DDoc = pSketchHoleDef->BeginEdit();
    p2DDoc->ksCircle(0, 0, d / 2.0, 1);
    pSketchHoleDef->EndEdit();

    ksEntityPtr pCut = m_part->NewEntity(o3d_cutExtrusion);
    ksCutExtrusionDefinitionPtr pCutDef = pCut->GetDefinition();
    pCutDef->directionType = dtReverse;
    pCutDef->SetSketch(pSketchHole);
    pCutDef->SetSideParam(true, etBlind, m, 0, false);
    pCut->Create();

    m_doc->SaveAs(L"C:\\Temp\\Гайка_ГОСТ15521.m3d");
}

void CKompasBuilder::CreateBoltGOST7817(const std::vector<double>& halfCouplingData, const CKURSACHDoc::BoltUIParams& ui)
{
    m_doc = m_app->Document3D();
    m_doc->Create(false, true);
    m_part = m_doc->GetPart(pTop_Part);

    // 7817: берём параметры из диалога (editbox-ов), чтобы не пересчитывать здесь
    const double hole = (ui.d2 > 0.0) ? ui.d2 : GetD(halfCouplingData, 14, 8.4);
    const int nominal = (ui.nominal > 0) ? ui.nominal : NominalFromHole(hole);

    const double S = (ui.S > 0) ? (double)ui.S : WrenchS(nominal);
    const double H_head = (ui.k > 0.0) ? ui.k : NutHeightM(nominal); // высота головки
    const double L1 = GetD(halfCouplingData, 19, 24.5); // l1
    const double L2 = GetD(halfCouplingData, 20, 15.0); // l2
    const int n = GetI(halfCouplingData, 15, 0);
    const double mNut = NutHeightM(nominal);

    // Итоговая длина болта берётся из диалога (там уже выполнен подбор по ГОСТ).
    const double L = (ui.l > 0) ? (double)ui.l : (2.0 * L1 + (double)n + mNut);

    const double D1 = GetD(halfCouplingData, 12, 9.0);      // d0
    const double D2 = (double)nominal;                      // номинал болта
    const double dHole = 2.0;                               // отверстие в болте (можно вынести параметром позже)
    const double r = 0.4;

    const double Hl1 = H_head + L1;
    const double Hl2 = H_head + L2;
    const double HL  = H_head + L;

    const double PI = 3.14159265358979323846;

    // Шестигранник
    ksEntityPtr pSketch1 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef1 = pSketch1->GetDefinition();
    pSketchDef1->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketch1->Create();
    ksDocument2DPtr p2DDoc = pSketchDef1->BeginEdit();
    const double R_out = S / std::sqrt(3.0);
    const double x1 = R_out * std::cos(PI / 6),  y1 = R_out * std::sin(PI / 6);
    const double x2 = R_out * std::cos(PI / 2),  y2 = R_out * std::sin(PI / 2);
    const double x3 = R_out * std::cos(5 * PI / 6), y3 = R_out * std::sin(5 * PI / 6);
    const double x4 = R_out * std::cos(7 * PI / 6), y4 = R_out * std::sin(7 * PI / 6);
    const double x5 = R_out * std::cos(3 * PI / 2), y5 = R_out * std::sin(3 * PI / 2);
    const double x6 = R_out * std::cos(11 * PI / 6), y6 = R_out * std::sin(11 * PI / 6);
    p2DDoc->ksLineSeg(x1, y1, x2, y2, 1);
    p2DDoc->ksLineSeg(x2, y2, x3, y3, 1);
    p2DDoc->ksLineSeg(x3, y3, x4, y4, 1);
    p2DDoc->ksLineSeg(x4, y4, x5, y5, 1);
    p2DDoc->ksLineSeg(x5, y5, x6, y6, 1);
    p2DDoc->ksLineSeg(x6, y6, x1, y1, 1);
    pSketchDef1->EndEdit();

    ksEntityPtr pExtrude1 = m_part->NewEntity(o3d_bossExtrusion);
    ksBossExtrusionDefinitionPtr pExtrudeDef1 = pExtrude1->GetDefinition();
    pExtrudeDef1->SetSketch(pSketch1);
    pExtrudeDef1->directionType = dtNormal;
    pExtrudeDef1->SetSideParam(true, etBlind, H_head, 0, false);
    pExtrude1->Create();

    // Цилиндр D2
    ksEntityPtr pSketch3 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef3 = pSketch3->GetDefinition();
    pSketchDef3->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketch3->Create();
    p2DDoc = pSketchDef3->BeginEdit();
    p2DDoc->ksCircle(0, 0, D2 / 2.0, 1);
    pSketchDef3->EndEdit();

    ksEntityPtr pExtrude3 = m_part->NewEntity(o3d_bossExtrusion);
    ksBossExtrusionDefinitionPtr pExtrudeDef3 = pExtrude3->GetDefinition();
    pExtrudeDef3->SetSketch(pSketch3);
    pExtrudeDef3->directionType = dtNormal;
    pExtrudeDef3->SetSideParam(true, etBlind, HL, 0, false);
    pExtrude3->Create();

    // Отверстие
    ksEntityPtr pSketch4 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef4 = pSketch4->GetDefinition();
    pSketchDef4->SetPlane(m_part->GetDefaultEntity(o3d_planeXOY));
    pSketch4->Create();
    p2DDoc = pSketchDef4->BeginEdit();
    p2DDoc->ksCircle(0, Hl1, dHole / 2.0, 1);
    pSketchDef4->EndEdit();

    ksEntityPtr pCut = m_part->NewEntity(o3d_cutExtrusion);
    ksCutExtrusionDefinitionPtr pCutDef = pCut->GetDefinition();
    pCutDef->directionType = dtMiddlePlane;
    pCutDef->SetSketch(pSketch4);
    pCutDef->SetSideParam(true, etBlind, D2, 0, false);
    pCut->Create();

    m_doc->SaveAs(L"C:\\Temp\\Болт_ГОСТ7817.m3d");
}

void CKompasBuilder::CreateBoltGOST7796(const std::vector<double>& halfCouplingData, const CKURSACHDoc::BoltUIParams& ui)
{
    m_doc = m_app->Document3D();
    m_doc->Create(false, true);
    m_part = m_doc->GetPart(pTop_Part);

    // 7796: берём параметры из диалога (editbox-ов)
    const double hole = (ui.d2 > 0.0) ? ui.d2 : GetD(halfCouplingData, 12, 9.0);
    const int nominal = (ui.nominal > 0) ? ui.nominal : NominalFromHole(hole);

    const double S = (ui.S > 0) ? (double)ui.S : WrenchS(nominal);
    const double k = (ui.k > 0.0) ? ui.k : NutHeightM(nominal);
    const double Dw = GetD(halfCouplingData, 13, 10.5); // d1 как диаметр подголовки (приближение)
    const double hw = 0.0;                      // В ДИАЛОГЕ 7796 hw нет (и здесь не используем)

    const int n = GetI(halfCouplingData, 15, 0);
    const double l1 = GetD(halfCouplingData, 19, 24.5);
    const double mNut = NutHeightM(nominal);
    const double l = (ui.l > 0) ? (double)ui.l : (2.0 * l1 + (double)n + mNut); // итоговая длина из диалога

    const double dBolt = (double)nominal;
    const double khw = k + hw;
    const double khwl = khw + l;

    const double PI = 3.14159265358979323846;

    // Шестигранник
    ksEntityPtr pSketch1 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef1 = pSketch1->GetDefinition();
    pSketchDef1->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketch1->Create();
    ksDocument2DPtr p2DDoc = pSketchDef1->BeginEdit();
    const double R_out = S / std::sqrt(3.0);
    const double x1 = R_out * std::cos(PI / 6),  y1 = R_out * std::sin(PI / 6);
    const double x2 = R_out * std::cos(PI / 2),  y2 = R_out * std::sin(PI / 2);
    const double x3 = R_out * std::cos(5 * PI / 6), y3 = R_out * std::sin(5 * PI / 6);
    const double x4 = R_out * std::cos(7 * PI / 6), y4 = R_out * std::sin(7 * PI / 6);
    const double x5 = R_out * std::cos(3 * PI / 2), y5 = R_out * std::sin(3 * PI / 2);
    const double x6 = R_out * std::cos(11 * PI / 6), y6 = R_out * std::sin(11 * PI / 6);
    p2DDoc->ksLineSeg(x1, y1, x2, y2, 1);
    p2DDoc->ksLineSeg(x2, y2, x3, y3, 1);
    p2DDoc->ksLineSeg(x3, y3, x4, y4, 1);
    p2DDoc->ksLineSeg(x4, y4, x5, y5, 1);
    p2DDoc->ksLineSeg(x5, y5, x6, y6, 1);
    p2DDoc->ksLineSeg(x6, y6, x1, y1, 1);
    pSketchDef1->EndEdit();

    ksEntityPtr pExtrude1 = m_part->NewEntity(o3d_bossExtrusion);
    ksBossExtrusionDefinitionPtr pExtrudeDef1 = pExtrude1->GetDefinition();
    pExtrudeDef1->SetSketch(pSketch1);
    pExtrudeDef1->directionType = dtNormal;
    pExtrudeDef1->SetSideParam(true, etBlind, k, 0, false);
    pExtrude1->Create();

    // Стержень
    ksEntityPtr pSketch3 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef3 = pSketch3->GetDefinition();
    pSketchDef3->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketch3->Create();
    p2DDoc = pSketchDef3->BeginEdit();
    p2DDoc->ksCircle(0, 0, dBolt / 2.0, 1);
    pSketchDef3->EndEdit();

    ksEntityPtr pExtrude3 = m_part->NewEntity(o3d_bossExtrusion);
    ksBossExtrusionDefinitionPtr pExtrudeDef3 = pExtrude3->GetDefinition();
    pExtrudeDef3->SetSketch(pSketch3);
    pExtrudeDef3->directionType = dtNormal;
    pExtrudeDef3->SetSideParam(true, etBlind, khwl, 0, false);
    pExtrude3->Create();

    m_doc->SaveAs(L"C:\\Temp\\Болт_ГОСТ7796.m3d");
}

void CKompasBuilder::CreateShaybaGOST6402(const std::vector<double>& halfCouplingData)
{
    // Проверка основных указателей
    if (!m_app) {
        AfxMessageBox(L"Ошибка: m_app не инициализирован");
        return;
    }
    m_doc = m_app->Document3D();
    if (!m_doc) {
        AfxMessageBox(L"Ошибка: не удалось получить документ");
        return;
    }

    // Создание документа и получение части
    m_doc->Create(false, true);
    m_part = m_doc->GetPart(pTop_Part);
    if (!m_part) {
        AfxMessageBox(L"Ошибка: не удалось получить часть");
        return;
    }

    const double hole = GetD(halfCouplingData, 12, 9.0);
    const int nominal = NominalFromHole(hole);
    const double d = (double)nominal;
    const double s = 1.6;
    const double b = 2.0;
    const double db = d + b;

    // ---- Построение геометрии ----
    ksEntityPtr pSketch1 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef1 = pSketch1->GetDefinition();
    pSketchDef1->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketch1->Create();
    ksDocument2DPtr p2DDoc1 = pSketchDef1->BeginEdit();
    p2DDoc1->ksCircle(0, 0, db / 2.0, 1);
    pSketchDef1->EndEdit();

    ksEntityPtr pSketch2 = m_part->NewEntity(o3d_sketch);
    ksSketchDefinitionPtr pSketchDef2 = pSketch2->GetDefinition();
    pSketchDef2->SetPlane(m_part->GetDefaultEntity(o3d_planeXOZ));
    pSketch2->Create();
    ksDocument2DPtr p2DDoc2 = pSketchDef2->BeginEdit();
    p2DDoc2->ksCircle(0, 0, d / 2.0, 1);
    pSketchDef2->EndEdit();

    ksEntityPtr pExtrude1 = m_part->NewEntity(o3d_bossExtrusion);
    ksBossExtrusionDefinitionPtr pExtrudeDef1 = pExtrude1->GetDefinition();
    pExtrudeDef1->SetSketch(pSketch1);
    pExtrudeDef1->directionType = dtNormal;
    pExtrudeDef1->SetSideParam(true, etBlind, s, 0, false);
    pExtrude1->Create();

    ksEntityPtr pExtrude2 = m_part->NewEntity(o3d_cutExtrusion);
    ksCutExtrusionDefinitionPtr pExtrudeDef2 = pExtrude2->GetDefinition();
    pExtrudeDef2->SetSketch(pSketch2);
    pExtrudeDef2->directionType = dtReverse;
    pExtrudeDef2->SetSideParam(true, etBlind, s, 0, false);
    pExtrude2->Create();

    // ---- Перестроение документа перед работой с гранями (как в вашем примере) ----
    m_doc->RebuildDocument();
    m_part->Update();

    // ---- Маркировка граней (аналогично WSCADDlg.cpp) ----
    ksEntityCollectionPtr allFaces = m_part->EntityCollection(o3d_face);
    if (!allFaces) {
        AfxMessageBox(L"Не удалось получить коллекцию граней");
        return;
    }
    long count = allFaces->GetCount();
    if (count == 0) {
        AfxMessageBox(L"Коллекция граней пуста");
        return;
    }

    // 1. Маркировка цилиндрической поверхности отверстия
    for (long i = 0; i < count; i++)
    {
        ksEntityPtr face = allFaces->GetByIndex(i);
        if (!face) continue;
        ksFaceDefinitionPtr def = face->GetDefinition();
        if (!def) continue;
        if (!def->IsCylinder()) continue;

        double height, radius;
        def->GetCylinderParam(&height, &radius);
        if (fabs(radius - d / 2.0) < 0.1)
        {
            face->Putname(L"HoleFace");
            face->Update();
            break;
        }
    }

    // Сохранение
    m_doc->SaveAs(L"C:\\Temp\\Шайба_ГОСТ6402.m3d");
}
void CKompasBuilder::CreateSborka()
{
    m_doc = m_app->Document3D();
    m_doc->Create(false, false);
    m_part = m_doc->GetPart(pTop_Part);

    m_doc->SetPartFromFile(L"C:\\Temp\\Полумуфта.m3d", m_part, true);
    m_doc->SetPartFromFile(L"C:\\Temp\\Полумуфта.m3d", m_part, true);
    m_doc->SetPartFromFile(L"C:\\Temp\\Болт_ГОСТ7817.m3d", m_part, true);
    m_doc->SetPartFromFile(L"C:\\Temp\\Болт_ГОСТ7796.m3d", m_part, true);
    m_doc->SetPartFromFile(L"C:\\Temp\\Гайка_ГОСТ15521.m3d", m_part, true);
    m_doc->SetPartFromFile(L"C:\\Temp\\Гайка_ГОСТ15521.m3d", m_part, true);
    m_doc->SetPartFromFile(L"C:\\Temp\\Шайба_ГОСТ6402.m3d", m_part, true);
    m_doc->SetPartFromFile(L"C:\\Temp\\Шайба_ГОСТ6402.m3d", m_part, true);

    m_part->Update();
    m_doc->SaveAs(L"C:\\Temp\\Сборка.m3d");
}
