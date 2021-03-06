/******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Crytek
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/


#pragma once

#include "core/resource_manager.h"

#include "driver/gl/gl_resources.h"

class WrappedOpenGL;

class GLResourceManager : public ResourceManager<GLResource, GLResourceRecord>
{
	public: 
		GLResourceManager(WrappedOpenGL *gl) : m_GL(gl) {}
		~GLResourceManager() {}

		void Shutdown()
		{
			while(!m_GLResourceRecords.empty())
			{
				auto it = m_GLResourceRecords.begin();
				ResourceId id = it->second->GetResourceID();
				it->second->Delete(this);

				if(!m_GLResourceRecords.empty() && m_GLResourceRecords.begin()->second->GetResourceID() == id)
					m_GLResourceRecords.erase(m_GLResourceRecords.begin());
			}

			m_CurrentResourceIds.clear();

			ResourceManager::Shutdown();
		}
		
		inline void RemoveResourceRecord(ResourceId id)
		{
			for(auto it = m_GLResourceRecords.begin(); it != m_GLResourceRecords.end(); it++)
			{
				if(it->second->GetResourceID() == id)
				{
					m_GLResourceRecords.erase(it);
					break;
				}
			}
			
			ResourceManager::RemoveResourceRecord(id);
		}

		ResourceId RegisterResource(GLResource res)
		{
			ResourceId id = TrackedResource::GetNewUniqueID();
			m_CurrentResourceIds[res] = id;
			AddCurrentResource(id, res);
			return id;
		}

		void UnregisterResource(GLResource res)
		{
			auto it = m_CurrentResourceIds.find(res);
			if(it != m_CurrentResourceIds.end())
			{
				ReleaseCurrentResource(it->second);
				m_CurrentResourceIds.erase(res);
			}
		}

		ResourceId GetID(GLResource res)
		{
			auto it = m_CurrentResourceIds.find(res);
			if(it != m_CurrentResourceIds.end())
				return it->second;
			return ResourceId();
		}

		GLResourceRecord *AddResourceRecord(ResourceId id)
		{
			GLResourceRecord *ret = ResourceManager::AddResourceRecord(id);
			GLResource res = GetCurrentResource(id);

			m_GLResourceRecords[res] = ret;

			return ret;
		}

		using ResourceManager::GetResourceRecord;

		GLResourceRecord *GetResourceRecord(GLResource res)
		{
			auto it = m_GLResourceRecords.find(res);
			if(it != m_GLResourceRecords.end())
				return it->second;

			return ResourceManager::GetResourceRecord(GetID(res));
		}

	private:
		bool SerialisableResource(ResourceId id, GLResourceRecord *record);
		
		bool ResourceTypeRelease(GLResource res) { return true; }

		bool Need_InitialState(GLResource res) { return false; }
		bool Need_InitialStateChunk(GLResource res) { return false; }
		bool Prepare_InitialState(GLResource res) { return true; }
		bool Serialise_InitialState(GLResource res) { return true; }
		void Create_InitialState(ResourceId id, GLResource live, bool hasData) { }
		void Apply_InitialState(GLResource live, GLResource initial, uint32_t count) { }

		map<GLResource, GLResourceRecord*> m_GLResourceRecords;

		map<GLResource, ResourceId> m_CurrentResourceIds;

		WrappedOpenGL *m_GL;
};

