/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __InstancedEntity_H__
#define __InstancedEntity_H__

#include "OgreMovableObject.h"

namespace Ogre
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Scene
	*  @{
	*/

	/** @See InstanceBatch to understand how instancing works.

		Instanced entities work in a very similar way as how an Entity works, as this interface
		tries to mimic it as most as possible to make the transition between Entity and InstancedEntity
		as straightforward and simple as possible.
		There are a couple inherent limitations, for example setRenderQueueGroup only works on
		the InstanceBatch level, not the individual instance. This limits Z sorting for alpha blending
		quite significantly
		An InstancedEntity won't appear in scene until a SceneNode is attached to them. Once the
		InstancedEntity is no longer needed, call InstanceBatch::removeInstancedEntity to put them
		back into a queue so the batch can return it back again when a new instance is requested.
		@par
		Internally, instanced entities that are not visible (i.e. by setting setVisible(false) or
		when they don't have a SceneNode attached to them) a Zero world matrix is sent to the vertex shader
		which in turn causes a zero area triangle.
		This obviously means no Vertex shader benefit, but saves a bit of pixel shader processing power.
		Also this means this object won't be considered when sizing the InstanceBatch's bounding box.
		@par
		Each instance has an instance ID unique within the batch, which matches the ID sent to the vertex
		shader by some techniques (like InstanceBatchShader).
		@par
		Differences between Entity and InstancedEntity:
		* Setting RenderQueueGroup and other Renderable stuff works at InstanceBatch level, not
		InstancedEntity. This is obviously a limitation from instancing in general, not this particular
		implementation

        @remarks
			Design discussion webpage
        @author
            Matias N. Goldberg ("dark_sylinc")
        @version
            1.0
     */
	class _OgreExport InstancedEntity : public Ogre::MovableObject
	{
		friend class InstanceBatch;
		friend class InstanceBatchShader;
		friend class InstanceBatchHW;
		friend class InstanceBatchHW_VTF;
		friend class BaseInstanceBatchVTF;

		uint16				m_instanceID;			//Note it may change after defragmenting!
		bool				m_inUse;
		InstanceBatch		*m_batchOwner;

		AnimationStateSet	*mAnimationState;
		SkeletonInstance	*m_skeletonInstance;
		Matrix4				*mBoneMatrices;			//Local space
		Matrix4				*mBoneWorldMatrices;	//World space
		Matrix4				mLastParentXform;
		unsigned long		mFrameAnimationLastUpdated;

		bool				mSharedTransform;		//When true, another InstancedEntity controls
													//the skeleton

		//Stores the master when we're the slave, store our slaves when we're the master
		typedef vector<InstancedEntity*>::type InstancedEntityVec;
		InstancedEntityVec m_sharingPartners;

		//Returns number of matrices writen to xform, assumes xform has enough space
		size_t getTransforms( Matrix4 *xform ) const;
		//Returns number of 32-bit values writen
		size_t getTransforms3x4( float *xform ) const;

		//Returns true if this InstancedObject is visible to the current camera
		bool findVisible( Camera *camera );

		//Creates/destroys our own skeleton, also tells slaves to unlink if we're destroying
		void createSkeletonInstance();
		void destroySkeletonInstance();

		//Just unlinks, and tells our master we're no longer sharing
		void unlinkTransform();

		//Called when a slave has unlinked from us
		void notifyUnlink( const InstancedEntity *slave );

		/// Incremented count for next name extension
        static NameGenerator msNameGenerator;

	public:
		InstancedEntity( InstanceBatch *batchOwner, uint32 instanceID );
		virtual ~InstancedEntity();

		/** Shares the entire transformation with another InstancedEntity. This is usefull when a mesh
			has more than one submeshes, therefore creating multiple InstanceManagers (one for each
			submesh). With this function, sharing makes the skeleton to be shared (less memory) and
			updated once (performance optimization).
			Note that one InstancedEntity (i.e. submesh 0) must be chosen as "master" which will share
			with the other instanced entities (i.e. submeshes 1-N) which are called "slaves"
			@par
			Requirements to share trasnformations:
				* Both InstancedEntities must have use the same skeleton
				* An InstancedEntity can't be both "master" and "slave" at the same time
			@remarks
			Sharing does nothing if the original mesh doesn't have a skeleton
			When an InstancedEntity is removed (@See InstanceBatch::removeInstancedEntity), it stops
			sharing the transform. If the instanced entity was the master one, all it's slaves stop
			sharing and start having their own transform too.
			@param slave The InstancedEntity that should share with us and become our slave
		*/
		void shareTransformWith( InstancedEntity *slave );

		/** @See shareTransformWith
			Stops sharing the transform if this is a slave, and notifies the master we're no longer
			a slave.
			If this is a master, tells all it's slave to stop sharing
			@remarks
			This function is automatically called in InstanceBatch::removeInstancedEntity
		*/
		void stopSharingTransform();

		InstanceBatch* _getOwner() const { return m_batchOwner; }

		const String& getMovableType(void) const;

		const AxisAlignedBox& getBoundingBox(void) const;
		Real getBoundingRadius(void) const;

		//This is used by our batch owner to get the closest entity's depth, returns infinity
		//when not attached to a scene node
		Real getSquaredViewDepth( const Camera* cam ) const;

		//Overriden so we can tell the InstanceBatch it needs to update it's bounds
		void _notifyMoved(void);
		void _notifyAttached( Node* parent, bool isTagPoint = false );

		//Do nothing, InstanceBatch takes care of this.
		void _updateRenderQueue( RenderQueue* queue )	{}
		void visitRenderables( Renderable::Visitor* visitor, bool debugRenderables = false ) {}

		/** @see Entity::hasSkeleton */
		bool hasSkeleton(void) const { return m_skeletonInstance != 0; }
		/** @see Entity::getSkeleton */
		SkeletonInstance* getSkeleton(void) const { return m_skeletonInstance; }

		/** @see Entity::getAnimationState */
		AnimationState* getAnimationState(const String& name) const;
		/** @see Entity::getAllAnimationStates */
		AnimationStateSet* getAllAnimationStates(void) const;

		/** Called by InstanceBatch in <i>his</i> _updateRenderQueue to tell us we need
			to calculate our bone matrices.
			@remarks Assumes it has a skeleton (m_skeletonInstance != 0)
			@returns true if something was actually updated
		*/
		bool _updateAnimation(void);
	};
}

#endif
