// Copyright (c) 2002 - 2019, Evil Quail LLC
// All Rights Reserved

#include <unify/Matrix.h>
#include <memory>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

class SpacialData
{
public:
	SpacialData( unify::Matrix original, unify::V3< float > axis, unify::TimeDelta rotation, unify::TimeDelta orbit, float daysOrbitParent )
		: m_matrix{ original }
		, m_axis{ axis }
		, m_rotation{ rotation }
		, m_orbit{ orbit }
		, m_daysOrbitParent{ daysOrbitParent }
	{
	}

	virtual ~SpacialData()
	{
	}

	virtual void Update(const UpdateParams & params, const SpacialData * parentData, float speedOfTime )
	{
		m_matrix = 
			unify::MatrixRotationAboutAxis( 
				unify::V3< float >::V3Y( 1.0f ), 
				unify::AngleInDegrees( params.GetDelta().GetSeconds() * m_orbit.GetSeconds() ) 
			)
			* m_matrix;
		
		m_matrix *= unify::MatrixTranslate( 
		{ 
			parentData ? parentData->GetMatrix().GetPosition() : unify::V3< float >::V3Zero() 
		} 
		);
		
		if ( m_daysOrbitParent != 0.0f )
		{
			m_matrix *= unify::MatrixRotationAboutAxis( 
				m_axis, 
				unify::AnglePI2() / m_daysOrbitParent
				* params.GetDelta().GetSeconds()
				* speedOfTime
				);
		}
	}

	unify::Matrix GetMatrix() const
	{
		return m_matrix;
	}

private:
	unify::V3< float > m_axis;
	unify::TimeDelta m_rotation;
	unify::TimeDelta m_orbit;
	unify::Matrix m_matrix;
	float m_hoursRotate;
	float m_daysOrbitParent;
};