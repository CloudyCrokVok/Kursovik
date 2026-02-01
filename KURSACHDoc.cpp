// KURSACHDoc.cpp: реализация класса CKURSACHDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "KURSACH.h"
#endif

#include "KURSACHDoc.h"
#include <cmath>  // Для fabs

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKURSACHDoc

IMPLEMENT_DYNCREATE(CKURSACHDoc, CDocument)

BEGIN_MESSAGE_MAP(CKURSACHDoc, CDocument)
END_MESSAGE_MAP()


// Создание или уничтожение CKURSACHDoc

CKURSACHDoc::CKURSACHDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора
	m_currentImagePath = _T("");
	m_execution = 16;  // По умолчанию исполнение 16
	m_variant = 1;     // По умолчанию вариант 1

	// Инициализация данных из JSON
	InitializeHalfCouplingData();
}

CKURSACHDoc::~CKURSACHDoc()
{
}

BOOL CKURSACHDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	// Сбрасываем путь к изображению
	m_currentImagePath = _T("");

	// Устанавливаем параметры по умолчанию
	m_execution = 16;
	m_variant = 1;

	return TRUE;
}

void CKURSACHDoc::InitializeHalfCouplingData()
{
	// Очищаем существующие данные
	m_halfCouplingData.clear();

	// Добавляем ВСЕ данные из JSON (все 70+ строк)

	// М=16
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 11, 1, 4, 12.8, 0.1, 1.0, 4, 0.5, 2, 80, 55, 18, 25, 8.4, 1, 9.0, 1, 25, 8, 2, 0.2, 0.33));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 11, 2, 4, 0.0, 0.1, 1.0, 4, 0.5, 2, 80, 55, 18, 25, 8.4, 1, 9.0, 1, 22, 8, 2, 0.2, 0.33));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 12, 1, 4, 13.8, 0.1, 1.0, 4, 0.5, 2, 80, 55, 20, 25, 8.4, 1, 9.0, 1, 32, 8, 2, 0.2, 0.35));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 12, 2, 4, 0.0, 0.1, 1.0, 4, 0.5, 2, 80, 55, 20, 25, 8.4, 1, 9.0, 1, 27, 8, 2, 0.2, 0.35));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 14, 1, 5, 16.3, 0.1, 1.0, 4, 0.5, 2, 80, 55, 22, 25, 8.4, 1, 9.0, 1, 32, 8, 2, 0.2, 0.35));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 14, 2, 5, 0.0, 0.1, 1.0, 4, 0.5, 2, 80, 55, 22, 25, 8.4, 1, 9.0, 1, 27, 8, 2, 0.2, 0.35));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 16, 1, 5, 18.3, 0.1, 1.0, 4, 0.5, 2, 80, 55, 25, 25, 8.4, 1, 9.0, 1, 42, 8, 2, 0.2, 0.38));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 16, 2, 5, 0.0, 0.1, 1.0, 4, 0.5, 2, 80, 55, 25, 25, 8.4, 1, 9.0, 1, 30, 8, 2, 0.2, 0.35));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 18, 1, 6, 20.8, 0.1, 1.0, 4, 0.5, 2, 80, 55, 28, 25, 8.4, 1, 9.0, 1, 42, 8, 2, 0.2, 0.4));
	m_halfCouplingData.push_back(HalfCouplingData(16.0, 18, 2, 6, 0.0, 0.1, 1.0, 4, 0.5, 2, 80, 55, 28, 25, 8.4, 1, 9.0, 1, 30, 8, 2, 0.2, 0.37));

	// М=31.5
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 16, 1, 5, 18.3, 0.1, 1.0, 4, 0.5, 2, 90, 60, 25, 30, 8.4, 1, 9.0, 1, 42, 10, 2, 0.2, 0.56));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 16, 2, 5, 0.0, 0.1, 1.0, 4, 0.5, 2, 90, 60, 25, 30, 8.4, 1, 9.0, 1, 30, 10, 2, 0.2, 0.53));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 18, 1, 5, 20.8, 0.1, 1.0, 4, 0.5, 2, 90, 60, 28, 30, 8.4, 1, 9.0, 1, 42, 10, 2, 0.2, 0.58));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 18, 2, 5, 0.0, 0.1, 1.0, 4, 0.5, 2, 90, 60, 28, 30, 8.4, 1, 9.0, 1, 30, 10, 2, 0.2, 0.56));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 19, 1, 6, 21.8, 0.1, 1.0, 4, 0.5, 2, 90, 60, 30, 30, 8.4, 1, 9.0, 1, 42, 10, 2, 0.2, 0.6));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 19, 2, 6, 0.0, 0.1, 1.0, 4, 0.5, 2, 90, 60, 30, 30, 8.4, 1, 9.0, 1, 30, 10, 2, 0.2, 0.55));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 20, 1, 6, 22.8, 0.1, 1.0, 4, 0.5, 2, 90, 60, 32, 30, 8.4, 1, 9.0, 1, 52, 10, 2, 0.2, 0.64));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 20, 2, 6, 0.0, 0.1, 1.0, 4, 0.5, 2, 90, 60, 32, 30, 8.4, 1, 9.0, 1, 38, 10, 2, 0.2, 0.59));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 22, 1, 6, 24.8, 0.1, 1.0, 4, 0.5, 2, 90, 60, 35, 30, 8.4, 1, 9.0, 1, 52, 10, 2, 0.2, 0.64));
	m_halfCouplingData.push_back(HalfCouplingData(31.5, 22, 2, 6, 0.0, 0.1, 1.0, 4, 0.5, 2, 90, 60, 35, 30, 8.4, 1, 9.0, 1, 38, 10, 2, 0.2, 0.59));

	// М=63
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 20, 1, 6, 22.8, 0.1, 1.0, 4, 0.5, 2, 100, 70, 32, 40, 8.4, 1, 9.0, 1, 52, 12, 2, 0.2, 0.88));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 20, 2, 6, 0.0, 0.1, 1.0, 4, 0.5, 2, 100, 70, 32, 40, 8.4, 1, 9.0, 1, 38, 12, 2, 0.2, 0.81));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 22, 1, 6, 24.8, 0.1, 1.0, 4, 0.5, 2, 100, 70, 35, 40, 8.4, 1, 9.0, 1, 52, 12, 2, 0.2, 0.88));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 22, 2, 6, 0.0, 0.1, 1.0, 4, 0.5, 2, 100, 70, 35, 40, 8.4, 1, 9.0, 1, 38, 12, 2, 0.2, 0.83));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 24, 1, 8, 27.3, 0.2, 1.0, 4, 0.5, 2, 100, 70, 38, 40, 8.4, 1, 9.0, 1, 52, 12, 2, 0.2, 0.93));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 24, 2, 8, 0.0, 0.2, 1.0, 4, 0.5, 2, 100, 70, 38, 40, 8.4, 1, 9.0, 1, 38, 12, 2, 0.2, 0.86));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 25, 1, 8, 28.3, 0.2, 1.0, 4, 0.5, 2, 100, 70, 40, 40, 8.4, 1, 9.0, 1, 62, 12, 2, 0.2, 1.0));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 25, 2, 8, 0.0, 0.2, 1.0, 4, 0.5, 2, 100, 70, 40, 40, 8.4, 1, 9.0, 1, 44, 12, 2, 0.2, 0.86));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 28, 1, 8, 31.3, 0.2, 1.0, 4, 0.5, 2, 100, 70, 42, 40, 8.4, 1, 9.0, 1, 62, 12, 2, 0.2, 0.94));
	m_halfCouplingData.push_back(HalfCouplingData(63.0, 28, 2, 8, 0.0, 0.2, 1.0, 4, 0.5, 2, 100, 70, 42, 40, 8.4, 1, 9.0, 1, 44, 12, 2, 0.2, 0.83));

	// М=125
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 25, 1, 8, 28.3, 0.2, 1.0, 4, 0.5, 2, 110, 80, 40, 45, 10.5, 1, 11.0, 1, 62, 15, 2, 0.2, 1.4));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 25, 2, 8, 0.0, 0.2, 1.0, 4, 0.5, 2, 110, 80, 40, 45, 10.5, 1, 11.0, 1, 44, 15, 2, 0.2, 1.2));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 28, 1, 8, 31.3, 0.2, 1.0, 4, 0.5, 2, 110, 80, 42, 45, 10.5, 1, 11.0, 1, 62, 15, 2, 0.2, 1.3));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 28, 2, 8, 0.0, 0.2, 1.0, 4, 0.5, 2, 110, 80, 42, 45, 10.5, 1, 11.0, 1, 44, 15, 2, 0.2, 1.2));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 30, 1, 8, 33.8, 0.2, 1.0, 4, 0.5, 2, 110, 80, 48, 45, 10.5, 1, 11.0, 1, 82, 15, 2, 0.2, 1.6));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 30, 2, 8, 33.3, 0.2, 1.0, 4, 0.5, 2, 110, 80, 48, 45, 10.5, 1, 11.0, 1, 60, 15, 2, 0.2, 1.4));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 32, 1, 10, 35.8, 0.2, 1.0, 4, 0.5, 2, 110, 80, 52, 45, 10.5, 1, 11.0, 1, 82, 15, 2, 0.4, 1.7));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 32, 2, 10, 35.3, 0.2, 1.0, 4, 0.5, 2, 110, 80, 52, 45, 10.5, 1, 11.0, 1, 60, 15, 2, 0.4, 1.5));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 35, 1, 10, 38.8, 0.2, 1.0, 4, 0.5, 2, 110, 80, 52, 45, 10.5, 1, 11.0, 1, 82, 15, 2, 0.4, 1.5));
	m_halfCouplingData.push_back(HalfCouplingData(125.0, 35, 2, 10, 38.3, 0.2, 1.0, 4, 0.5, 2, 110, 80, 52, 45, 10.5, 1, 11.0, 1, 60, 15, 2, 0.4, 1.4));

	// М=160
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 30, 1, 8, 33.8, 0.2, 1.0, 4, 0.5, 2, 120, 100, 48, 50, 10.5, 1, 11.0, 1, 82, 16, 2, 0.2, 1.9));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 30, 2, 8, 33.3, 0.2, 1.0, 4, 0.5, 2, 120, 100, 48, 50, 10.5, 1, 11.0, 1, 60, 16, 2, 0.2, 1.7));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 32, 1, 8, 35.8, 0.2, 1.0, 4, 0.5, 2, 120, 100, 52, 50, 10.5, 1, 11.0, 1, 82, 16, 2, 0.2, 2.0));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 32, 2, 8, 35.3, 0.2, 1.0, 4, 0.5, 2, 120, 100, 52, 50, 10.5, 1, 11.0, 1, 60, 16, 2, 0.2, 1.8));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 35, 1, 10, 38.8, 0.2, 1.0, 4, 0.5, 2, 120, 100, 52, 50, 10.5, 1, 11.0, 1, 82, 16, 2, 0.4, 1.9));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 35, 2, 10, 38.3, 0.2, 1.0, 4, 0.5, 2, 120, 100, 52, 50, 10.5, 1, 11.0, 1, 60, 16, 2, 0.4, 1.7));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 36, 1, 10, 39.8, 0.2, 1.0, 4, 0.5, 2, 120, 100, 52, 50, 10.5, 1, 11.0, 1, 82, 16, 2, 0.4, 1.8));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 36, 2, 10, 39.3, 0.2, 1.0, 4, 0.5, 2, 120, 100, 52, 50, 10.5, 1, 11.0, 1, 60, 16, 2, 0.4, 1.7));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 38, 1, 10, 41.8, 0.2, 1.0, 4, 0.5, 2, 120, 100, 56, 50, 10.5, 1, 11.0, 1, 82, 16, 2, 0.4, 2.0));
	m_halfCouplingData.push_back(HalfCouplingData(160.0, 38, 2, 10, 41.3, 0.2, 1.0, 4, 0.5, 2, 120, 100, 56, 50, 10.5, 1, 11.0, 1, 60, 16, 2, 0.4, 1.8));

	// М=250
	m_halfCouplingData.push_back(HalfCouplingData(250.0, 32, 1, 10, 35.8, 0.2, 1.0, 4, 0.5, 2, 130, 110, 52, 55, 10.5, 1, 11.0, 1, 82, 17, 2, 0.4, 2.5));
	m_halfCouplingData.push_back(HalfCouplingData(250.0, 32, 2, 10, 35.3, 0.2, 1.0, 4, 0.5, 2, 130, 110, 52, 55, 10.5, 1, 11.0, 1, 60, 17, 2, 0.4, 2.3));
	m_halfCouplingData.push_back(HalfCouplingData(250.0, 35, 1, 10, 38.8, 0.2, 1.0, 4, 0.5, 2, 130, 110, 52, 55, 10.5, 1, 11.0, 1, 82, 17, 2, 0.4, 2.4));
	m_halfCouplingData.push_back(HalfCouplingData(250.0, 35, 2, 10, 38.3, 0.2, 1.0, 4, 0.5, 2, 130, 110, 52, 55, 10.5, 1, 11.0, 1, 60, 17, 2, 0.4, 2.2));
	m_halfCouplingData.push_back(HalfCouplingData(250.0, 36, 1, 10, 39.8, 0.2, 1.0, 4, 0.5, 2, 130, 110, 52, 55, 10.5, 1, 11.0, 1, 82, 17, 2, 0.4, 2.3));
	m_halfCouplingData.push_back(HalfCouplingData(250.0, 36, 2, 10, 39.3, 0.2, 1.0, 4, 0.5, 2, 130, 110, 52, 55, 10.5, 1, 11.0, 1, 60, 17, 2, 0.4, 2.2));
	m_halfCouplingData.push_back(HalfCouplingData(250.0, 38, 1, 10, 41.8, 0.2, 1.0, 4, 0.5, 2, 130, 110, 56, 55, 10.5, 1, 11.0, 1, 82, 17, 2, 0.4, 2.4));
	m_halfCouplingData.push_back(HalfCouplingData(250.0, 38, 2, 10, 41.3, 0.2, 1.0, 4, 0.5, 2, 130, 110, 56, 55, 10.5, 1, 11.0, 1, 60, 17, 2, 0.4, 2.2));

	// М=400
	m_halfCouplingData.push_back(HalfCouplingData(400.0, 35, 1, 10, 38.8, 0.2, 1.6, 4, 1.0, 2, 140, 110, 52, 65, 10.5, 1, 11.0, 1, 82, 17, 2, 0.4, 2.7));
	m_halfCouplingData.push_back(HalfCouplingData(400.0, 35, 2, 10, 38.3, 0.2, 1.6, 4, 1.0, 2, 140, 110, 52, 65, 10.5, 1, 11.0, 1, 60, 17, 2, 0.4, 2.5));
	m_halfCouplingData.push_back(HalfCouplingData(400.0, 36, 1, 10, 39.8, 0.2, 1.6, 4, 1.0, 2, 140, 110, 52, 65, 10.5, 1, 11.0, 1, 82, 17, 2, 0.4, 2.7));
	m_halfCouplingData.push_back(HalfCouplingData(400.0, 36, 2, 10, 39.3, 0.2, 1.6, 4, 1.0, 2, 140, 110, 52, 65, 10.5, 1, 11.0, 1, 60, 17, 2, 0.4, 2.6));
	m_halfCouplingData.push_back(HalfCouplingData(400.0, 38, 1, 10, 41.8, 0.2, 1.6, 4, 1.0, 2, 140, 110, 56, 65, 10.5, 1, 11.0, 1, 82, 17, 2, 0.4, 2.7));
	m_halfCouplingData.push_back(HalfCouplingData(400.0, 38, 2, 10, 41.3, 0.2, 1.6, 4, 1.0, 2, 140, 110, 56, 65, 10.5, 1, 11.0, 1, 60, 17, 2, 0.4, 2.5));
}

void CKURSACHDoc::SetHalfCouplingParams(int execution, int variant)
{
	m_execution = execution;
	m_variant = variant;

	// Обновляем представления
	UpdateAllViews(NULL);
}

std::vector<double> CKURSACHDoc::GetHalfCouplingData(int execution, int variant)
{
	std::vector<double> result;

	// ВАЖНОЕ ИСПРАВЛЕНИЕ: преобразуем целое число исполнения в double
	// Так как в структуре HalfCouplingData поле M имеет тип double
	double executionD = (double)execution;

	// Для исполнения 31 нужно преобразовать в 31.5
	if (execution == 31) {
		executionD = 31.5;
	}

	// Поиск данных по исполнению и варианту
	for (int i = 0; i < (int)m_halfCouplingData.size(); i++)
	{
		const HalfCouplingData& data = m_halfCouplingData[i];

		// Используем точное сравнение для double
		if (fabs(data.M - executionD) < 0.0001 && data.exec == variant)
		{
			// Возвращаем все параметры в виде вектора
			result.push_back(data.M);
			result.push_back(data.d);
			result.push_back((double)data.exec);
			result.push_back(data.B);
			result.push_back(data.h);
			result.push_back(data.h_dev);
			result.push_back(data.b1_num);
			result.push_back(data.b1_den);
			result.push_back(data.b2_num);
			result.push_back(data.b2_den);
			result.push_back(data.D);
			result.push_back(data.D1);
			result.push_back(data.d0);
			result.push_back(data.d1);
			result.push_back(data.d2);
			result.push_back((double)data.n);
			result.push_back(data.d4);
			result.push_back((double)data.n3);
			result.push_back(data.l);
			result.push_back(data.l1);
			result.push_back(data.l2);
			result.push_back(data.R);
			result.push_back(data.mass);

			// Нашли нужные данные - выходим из цикла
			break;
		}
	}

	// Если данные не найдены, возвращаем пустой вектор
	return result;
}


// Сериализация CKURSACHDoc

void CKURSACHDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CKURSACHDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CKURSACHDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CKURSACHDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CKURSACHDoc

#ifdef _DEBUG
void CKURSACHDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKURSACHDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CKURSACHDoc
