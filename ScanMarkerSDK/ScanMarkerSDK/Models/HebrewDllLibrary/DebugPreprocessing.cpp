#include "DebugPreprocessing.h"

CDebugPreprocessing::CDebugPreprocessing(void)
{
	m_preprocessingType = DOUBLE_THICK;
	m_pDataIn = NULL;;
	m_pDataOut = NULL;
	m_width = 0;
	m_height = 0;
	m_output_width = 0;
	
}

CDebugPreprocessing::~CDebugPreprocessing(void)
{
	m_preprocessingType = DOUBLE_THICK;
	m_pDataIn = NULL;;
	m_pDataOut = NULL;
	m_width = 0;
	m_height = 0;
	m_output_width = 0;

}
void CDebugPreprocessing::Init(int width, int height, byte* pDataIn, int output_width, byte* pDataOut)
{
	m_pDataIn = pDataIn;
	m_pDataOut = pDataOut;
	m_width = width;
	m_height = height;
	m_output_width = output_width;
	
}
void CDebugPreprocessing::Calculate()
{
	switch (m_preprocessingType )
	{
		case DOUBLE_THICK:
		{
			DoubleThickPreprocessing();
			break;
		}
		case NO_PREPROCESSING:
		{
			NoPreprocessing();
			break;
		}

	}
}
void CDebugPreprocessing::DoubleThickPreprocessing()
{
	int indOutput = 0;
	bool inc_i = true;
	for ( int i = 0; i < m_width; )
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_output_width*3+i*3+0]  = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_output_width*3+i*3+1]  = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_output_width*3+i*3+2]  = m_pDataIn[j*m_width+i];

		}
		if ( indOutput < m_output_width-1)
		{
			indOutput++;
		}

		inc_i = !inc_i;
		if ( inc_i) i++;
	}

}
void CDebugPreprocessing::NoPreprocessing()
{
	for ( int i = 0; i < m_width; i++)
	{
		for ( int j = 0; j < m_height; j++)
		{
			m_pDataOut[j*m_width*3+i*3+0] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3+i*3+1] = m_pDataIn[j*m_width+i];
			m_pDataOut[j*m_width*3+i*3+2] = m_pDataIn[j*m_width+i];

		}
	}
}
	