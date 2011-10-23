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
#ifndef __D3D11HLSLProgram_H__
#define __D3D11HLSLProgram_H__

#include "OgreD3D11Prerequisites.h"
#include "OgreHighLevelGpuProgram.h"
#include "OgreD3D11VertexDeclaration.h"


namespace Ogre {
	/** Specialization of HighLevelGpuProgram to provide support for D3D11 
	High-Level Shader Language (HLSL).
	@remarks
	Note that the syntax of D3D11 HLSL is identical to nVidia's Cg language, therefore
	unless you know you will only ever be deploying on Direct3D, or you have some specific
	reason for not wanting to use the Cg plugin, I suggest you use Cg instead since that
	can produce programs for OpenGL too.
	*/
	class D3D11HLSLProgram : public HighLevelGpuProgram
	{
	public:
		/// Command object for setting entry point
		class CmdEntryPoint : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		/// Command object for setting target assembler
		class CmdTarget : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		/// Command object for setting macro defines
		class CmdPreprocessorDefines : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		/// Command object for setting matrix packing in column-major order
		class CmdColumnMajorMatrices : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		/// Command object for setting backwards compatibility
		class CmdEnableBackwardsCompatibility : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

	protected:

		static CmdEntryPoint msCmdEntryPoint;
		static CmdTarget msCmdTarget;
		static CmdPreprocessorDefines msCmdPreprocessorDefines;
		static CmdColumnMajorMatrices msCmdColumnMajorMatrices;
		static CmdEnableBackwardsCompatibility msCmdEnableBackwardsCompatibility;
		

		/** Internal method for creating an appropriate low-level program from this
		high-level program, must be implemented by subclasses. */
		void createLowLevelImpl(void);
		/// Internal unload implementation, must be implemented by subclasses
		void unloadHighLevelImpl(void);
		/// Populate the passed parameters with name->index map, must be overridden
		//void populateParameterNames(GpuProgramParametersSharedPtr params);

		// Recursive utility method for populateParameterNames
		void processParamElement(String prefix, LPCSTR pName, size_t paramIndex, ID3D11ShaderReflectionType* varRefType) const;

		void populateDef(D3D11_SHADER_TYPE_DESC& d3dDesc, GpuConstantDefinition& def) const;

		String mTarget;
		String mEntryPoint;
		String mPreprocessorDefines;
		bool mColumnMajorMatrices;
		bool mEnableBackwardsCompatibility;

		bool mErrorsInCompile;
		ID3D10Blob * mpMicroCode;
		ID3D11Buffer* mConstantBuffer;
		
		D3D11Device & mDevice;

		ID3D11ShaderReflectionConstantBuffer* mShaderReflectionConstantBuffer;
		D3D11_SHADER_BUFFER_DESC mConstantBufferDesc ;
		D3D11_SHADER_DESC mShaderDesc;

		D3D11VertexDeclaration mInputVertexDeclaration;

		ID3D11ShaderReflection* mpIShaderReflection;

		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;
		ID3D11GeometryShader* mpGeometryShader;

		struct ShaderVarWithPosInBuf
		{
			bool wasInit;
			bool isFloat;
			size_t physicalIndex;
			void * src;
			String name;

			D3D11_SHADER_VARIABLE_DESC var;
		};
		typedef vector<ShaderVarWithPosInBuf>::type ShaderVars;
		typedef ShaderVars::iterator ShaderVarsIter; 

		ShaderVars mShaderVars;

		void createConstantBuffer(const UINT ByteWidth);
		void analizeMicrocode();
		void getMicrocodeFromCache(void);
		void compileMicrocode(void);
	public:
		D3D11HLSLProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
			const String& group, bool isManual, ManualResourceLoader* loader, D3D11Device & device);
		~D3D11HLSLProgram();

		/** Sets the entry point for this program ie the first method called. */
		void setEntryPoint(const String& entryPoint) { mEntryPoint = entryPoint; }
		/** Gets the entry point defined for this program. */
		const String& getEntryPoint(void) const { return mEntryPoint; }
		/** Sets the shader target to compile down to, e.g. 'vs_1_1'. */
		void setTarget(const String& target);
		/** Gets the shader target to compile down to, e.g. 'vs_1_1'. */
		const String& getTarget(void) const { return mTarget; }
		/** Sets the preprocessor defines use to compile the program. */
		void setPreprocessorDefines(const String& defines) { mPreprocessorDefines = defines; }
		/** Sets the preprocessor defines use to compile the program. */
		const String& getPreprocessorDefines(void) const { return mPreprocessorDefines; }
		/** Sets whether matrix packing in column-major order. */ 
		void setColumnMajorMatrices(bool columnMajor) { mColumnMajorMatrices = columnMajor; }
		/** Gets whether matrix packed in column-major order. */
		bool getColumnMajorMatrices(void) const { return mColumnMajorMatrices; }
		/** Sets whether backwards compatibility is enabled. */ 
		void setEnableBackwardsCompatibility(bool enableBackwardsCompatibility) { mEnableBackwardsCompatibility = enableBackwardsCompatibility; }
		/** Gets whether backwards compatibility is enabled. */
		bool getEnableBackwardsCompatibility(void) const { return mEnableBackwardsCompatibility; }
		/// Overridden from GpuProgram
		bool isSupported(void) const;
		/// Overridden from GpuProgram
		GpuProgramParametersSharedPtr createParameters(void);
		/// Overridden from GpuProgram
		const String& getLanguage(void) const;

		virtual void buildConstantDefinitions() const;
		ID3D11VertexShader* getVertexShader(void) const;
		ID3D11PixelShader* getPixelShader(void) const; 
		ID3D11GeometryShader* getGeometryShader(void) const; 
		ID3D10Blob * getMicroCode(void) const;  

		ID3D11Buffer* getConstantBuffer(GpuProgramParametersSharedPtr params, uint16 variabilityMask);

		void CreateVertexShader();
		void CreatePixelShader();
		void CreateGeometryShader();

		/** Internal load implementation, must be implemented by subclasses.
		*/
		void loadFromSource(void);

		D3D11VertexDeclaration & getInputVertexDeclaration() { return mInputVertexDeclaration; }
	};
}

#endif
