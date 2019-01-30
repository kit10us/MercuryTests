// Copyright (c) 2002 - 2019, Evil Quail LLC
// All Rights Reserved`

#pragma once
#include <SpacialData.h>

class SolarBody
{
public:
	typedef std::shared_ptr< SolarBody > ptr;
	typedef std::shared_ptr< const SolarBody > const_ptr;

	SolarBody(std::string name, Object * object, const SpacialData startSpacialData )
		: m_name{ name }
		, m_object{ object }
		, m_startSpacialData{ startSpacialData }
		, m_spacialData{ startSpacialData }
	{
	}

	// <Description>
	// Resets spacial data.
	// </Description>
	void Reset()
	{
		m_spacialData = m_startSpacialData;
		for ( auto child : m_children )
		{
			child->Reset();
		}
	}

	void AddChild( SolarBody::ptr child )
	{
		m_children.push_back( child );
	}

	void Update( const UpdateParams & params, const SpacialData * parentSpacialData, float speedOfTime )
	{
		// Updates self...
		m_spacialData.Update( params, parentSpacialData, speedOfTime );
		m_object->GetFrame().SetMatrix( m_spacialData.GetMatrix() );

		// Update children...
		for ( auto & child : m_children )
		{
			child->Update( params, &m_spacialData, speedOfTime );
		}
	}

private:
	std::string m_name;
	Object * m_object;
	SpacialData m_startSpacialData;
	SpacialData m_spacialData;
	std::list< SolarBody::ptr > m_children;
};