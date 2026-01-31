// KURSACHDoc.h: интерфейс класса CKURSACHDoc 
//

#pragma once

#include <vector>  // Добавляем для std::vector

class CMyTreeView;
class CKURSACHView;


class CKURSACHDoc : public CDocument
{
protected: // создать только из сериализации
	CKURSACHDoc() noexcept;
	DECLARE_DYNCREATE(CKURSACHDoc)

	// Атрибуты
public:
	CKURSACHView* pView;
	CMyTreeView* pTree;

	// Операции
public:
	// Добавляем методы для работы с изображением
	void SetCurrentImagePath(const CString& path) { m_currentImagePath = path; }
	CString GetCurrentImagePath() const { return m_currentImagePath; }

	// Добавляем методы для работы с параметрами полумуфты
	void SetHalfCouplingParams(int execution, int variant);
	void GetHalfCouplingParams(int& execution, int& variant) const
	{
		execution = m_execution;
		variant = m_variant;
	}

	// Данные полумуфты
	std::vector<double> GetHalfCouplingData(int execution, int variant);

	// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Реализация
public:
	virtual ~CKURSACHDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Добавляем поля для хранения данных
	CString m_currentImagePath;
	int m_execution;
	int m_variant;

	// Структура для хранения данных полумуфты
	struct HalfCouplingData
	{
		double M;
		double d;
		int exec;
		double B;
		double h;
		double h_dev;
		double b1_num;
		double b1_den;
		double b2_num;
		double b2_den;
		double D;
		double D1;
		double d0;
		double d1;
		double d2;
		int n;
		double d4;
		int n3;
		double l;
		double l1;
		double l2;
		double R;
		double mass;

		HalfCouplingData(double M_, double d_, int exec_, double B_, double h_, double h_dev_,
			double b1_num_, double b1_den_, double b2_num_, double b2_den_,
			double D_, double D1_, double d0_, double d1_, double d2_, int n_,
			double d4_, int n3_, double l_, double l1_, double l2_, double R_,
			double mass_) :
			M(M_), d(d_), exec(exec_), B(B_), h(h_), h_dev(h_dev_),
			b1_num(b1_num_), b1_den(b1_den_), b2_num(b2_num_), b2_den(b2_den_),
			D(D_), D1(D1_), d0(d0_), d1(d1_), d2(d2_), n(n_),
			d4(d4_), n3(n3_), l(l_), l1(l1_), l2(l2_), R(R_), mass(mass_) {
		}
	};

	std::vector<HalfCouplingData> m_halfCouplingData;

	// Инициализация данных
	void InitializeHalfCouplingData();

	// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
