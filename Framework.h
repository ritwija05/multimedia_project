
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#define FRAMEWORK_VER "3.30"

// --------------------------------------------------------
// General Guidelines
//
// Using C-Strings for the project
//       You need to set your project settings
//       Properties / General / Advanced / Character Set->Use Multi - Byte Character Set
//
// Forced include this this header
//		Properties/C++/Advanced/Forced Include File -> "Framework.h"
//      Make sure Framework.h is any project include directory
//
// Add to your defines:
//      Properties/C++/Preprocessor/Preprocessor Definition 
//           -> add WINDOWS_TARGET_PLATFORM="$(TargetPlatformVersion)";
//           -> add MEMORY_LOGS_DIR=R"($(SolutionDir))";
//           -> add TOOLS_VERSION=R"($(VCToolsVersion))";
//
// Turn off Memory Tracking...
//     Comment out the #define MEM_TRACKING_ENABLED
// 
// --------------------------------------------------------

// Comment out this line to turn off memory tracking
#define MEM_TRACKER_ENABLED

// default warning... please setup correctly
#ifndef WINDOWS_TARGET_PLATFORM
	#define  WINDOWS_TARGET_PLATFORM "- MISSING - "
#endif

#ifndef TOOLS_VERSION
	#define  TOOLS_VERSION "- MISSING - "
#endif

// Build Mode:
#ifdef _DEBUG
	#ifdef _M_X64
		#define BUILD_CONFIG_MODE "x64 Debug  "
	#else
		#define BUILD_CONFIG_MODE "x86 Debug  "
	#endif
#else
	#ifdef _M_X64
		#define BUILD_CONFIG_MODE "x64 Release"
	#else
		#ifdef MR_FAST   // Only used in optimized class
			#define BUILD_CONFIG_MODE "MR_FAST    "
		#else
			#define BUILD_CONFIG_MODE "x86 Release"
		#endif
	#endif
#endif

// -----------------------------------------------------------------------------
// Warning levels turned on for Wall... that Should be supressed globally
//     Set here to avoid surpressing them in the settings
// -----------------------------------------------------------------------------

#pragma warning( disable : 4127 ) // conditional expression is constant
#pragma warning( disable : 4201 ) // nonstandard extension used: nameless struct/union
#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable : 4514 ) // 'function': unreferenced inline function has been removed
#pragma warning( disable : 4571 ) // Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#pragma warning( disable : 4625 ) // 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
#pragma warning( disable : 4626 ) // 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
#pragma warning( disable : 4668 ) // 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directive'
#pragma warning( disable : 4587 ) // 'anonymous_structure': behavior change: constructor is no longer implicitly called
#pragma warning( disable : 4588 ) // 'anonymous_structure': behavior change: destructor is no longer implicitly called
#pragma warning( disable : 4710 ) // 'function': function not inlined
#pragma warning( disable : 4711 ) // function 'function' selected for automatic inline expansion
#pragma warning( disable : 4820 ) // 'bytes' bytes padding added after construct 'member_name'
#pragma warning( disable : 4291 ) // 'declaration' : no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning( disable : 5025 ) // 'type': move assignment operator was implicitly defined as deleted
#pragma warning( disable : 5026 ) // 'type': move constructor was implicitly defined as deleted
#pragma warning( disable : 5027 ) // 'type': move assignment operator was implicitly defined as deleted
#pragma warning( disable : 5039 ) // function': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception.
#pragma warning( disable : 5045 ) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning( disable : 6011 ) // dereferencing NULL pointer <name>

#include "ProjectSettings.h"

// --------------------------------------
// General includes
// --------------------------------------

#include <assert.h>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>  
#include <crtdbg.h>  


// -----------------------------------------------------
// Many standard includes have serious warnings 
//     need to wrap for Wall warnings
//
// Todo: reverify with major compiler releases... 
//       Come on Microsoft.. clean up your code
// -----------------------------------------------------

#pragma warning( push )
	#pragma warning( disable : 4820 ) // 'bytes' bytes padding added after construct 'member_name'
	#define NOMINMAX
	#include <Windows.h>
#pragma warning( pop ) 

#pragma warning( push )
	#pragma warning( disable : 4774 )
	#include <string>
#pragma warning( pop ) 

#pragma warning( push )
	#pragma warning( disable : 4365 ) // 'classname': class has virtual functions, but destructor is not virtual\n instances of this class may not be destructed correctly
	#pragma warning( disable : 4623 ) // 'derived class': default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
	#pragma warning( disable : 4625 ) // 'derived class': copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
	#pragma warning( disable : 4626 ) // 'derived class': assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
	#pragma warning( disable : 4820 ) // 'bytes' bytes padding added after construct 'member_name'
	#pragma warning( disable : 5026 ) // 'type': move constructor was implicitly defined as deleted
	#pragma warning( disable : 5027 ) // 'type': move assignment operator was implicitly defined as deleted
	#include <mutex>
#pragma warning( pop ) 

#pragma warning( push )
	#pragma warning( disable : 4365 )  // 'classname': class has virtual functions, but destructor is not virtual\n instances of this class may not be destructed correctly
	#pragma warning( disable : 4625 ) // 'derived class': copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
	#pragma warning( disable : 4626 ) // 'derived class': assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
	#pragma warning( disable : 5026 ) // 'type': move constructor was implicitly defined as deleted
	#pragma warning( disable : 5027 ) // 'type': move assignment operator was implicitly defined as deleted
	#include <atomic>
#pragma warning( pop ) 

#define AZUL_REPLACE_ME_STUB(x) (x)
#define AZUL_REPLACE_ME(x) (void(x))
#define AZUL_INSERT_CODE_HERE             // do nothing
#define AZUL_UNUSED_VAR(x) (void(x))


#ifndef MEM_TRACKER_H
#define MEM_TRACKER_H

		// ---------------------------------------------------------------------------
		// One instructions to rule them all:
		//
		//   To Enable: 
		//           #define MEM_TRACKER_ENABLED
		//   To Disable: 
		//           Comment out #define MEM_TRACKER_ENABLED
		//
		//      Include Framework.h to every file:
		//           Change your project settings
		//               Go to C / C++ tab
		//               Go to Advanced
		//                      set Forced Include : Framework.h
		//               This will add this file automagically to all files (great!)
		//
		//  placement new() label:
        //      
		//      No memory tracking __SHOULD__ take place on placement new
		//      So you need to tell the Memory Tracking system to ignore this placement new
		//      Add 3 lines around placement new to allow it to compile
		//
		//      example:  Dog *pDog = new(address) Dog();  // original (new way below)
		//
		//      #pragma warning( push )   <----- Add 1/3 --------------
		//      #undef new                <----- Add 2/3 --------------
		//
		//              Dog *pDog = new(address) Dog();
		//
		//      #pragma warning( pop )    <----- Add 3/3 --------------
		//
		//   Issue with new(std::nothrow)
		//      This needs to be tracked... but the macro trick is a problem
		//      
		// ---------------------------------------------------------------------------

		enum class MemTracker
		{
			Disabled
		};

		#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
			#define MEMORY_TRACKING_ENABLE_STRING "enabled         "
		#else
			#define MEMORY_TRACKING_ENABLE_STRING "--> DISABLED <--"
		#endif

		class MemTrace
		{
		private:
			static const unsigned int MemTraceBuffSize = 256;
			static const unsigned int MemStringSize = 512;

		public:
			// -----------------------------------------------------
			// Only found this structure in documentation
			// created a shadow structure to allow error reporting 
			// -----------------------------------------------------
			typedef struct _CrtMemBlockHeader
			{
				static const unsigned int nNoMansLandSize = 4; // said in doc that its 4

				struct _CrtMemBlockHeader *pBlockHeaderNext;
				struct _CrtMemBlockHeader *pBlockHeaderPrev;
				char *szFileName;    // File name
				int nLine;           // Line number
				int nBlockUse;       // Type of block   
				size_t nDataSize;    // Size of user block
				long lRequest;       // Allocation number
				unsigned char gap[nNoMansLandSize];

				// ---------------------------------------------------
				// In an actual memory block in the debug heap,
				//    this structure is followed by:
				//       unsigned char data[nDataSize];
				//       unsigned char anotherGap[nNoMansLandSize];
				// ---------------------------------------------------

			} _CrtMemBlockHeader;

		public:
			// Big Six
			MemTrace() noexcept
			{
				memset(&privBuff[0], 0, MemTraceBuffSize);
			}
			MemTrace(const MemTrace &) = delete;
			MemTrace(MemTrace &&) = delete;
			MemTrace & operator = (const MemTrace &) = delete;
			MemTrace & operator = (MemTrace &&) = delete;
			~MemTrace() = default;

		private:
			// displays a printf to the output window
			void privOut(const char * const fmt, ...)
			{
				MemTrace *pTrace = MemTrace::privGetInstance();
				assert(pTrace);

				pTrace->mtx.lock();

				va_list args;

				#pragma warning( push )
					#pragma warning( disable : 26492 )
					#pragma warning( disable : 26481 )
					va_start(args, fmt);
				#pragma warning( pop )

				vsprintf_s(&pTrace->privBuff[0], MemTraceBuffSize, fmt, args);
				OutputDebugString(&pTrace->privBuff[0]);

				// va_end(args); - original.. below to new code
				args = static_cast<va_list> (nullptr);

				pTrace->mtx.unlock();
			}

			char *privStripDir( const char * const pInName) noexcept
			{
				char *pReturn = (char *)pInName;

				const char *pch = pInName;

				while (pch != nullptr)
				{
					pch = strstr(pch, "\\");
					if (pch != nullptr)
					{
						pch += 1;
						pReturn = (char *)pch;
					}
				}

				return pReturn;
			}

			void privDisplayLogLink()
			{
				char sBuff[MemTrace::MemStringSize] = { 0 };
				GetCurrentDirectory(MemTrace::MemStringSize, sBuff);

				const char *pch = sBuff;
				char *pSlash = nullptr;

				while (pch != nullptr)
				{
					pch = strstr(pch, "\\");
					if (pch != nullptr)
					{
						pch += 1;
						pSlash = (char *)pch;
					}
				}

				const size_t numBytes = (size_t)(pSlash - sBuff);

				char pBuff[MemTrace::MemStringSize] = { 0 };
				memcpy_s(pBuff, MemTrace::MemStringSize, sBuff, numBytes);

#ifndef MEMORY_LOGS_DIR
				char pString[MemTrace::MemStringSize] = "";
				strcat_s(pString, MemTrace::MemStringSize, "Logs\\MemTrackerLog.txt");
#else
				char pString[MemTrace::MemStringSize] = MEMORY_LOGS_DIR;
				strcat_s(pString, MemTrace::MemStringSize, "Logs\\MemTrackerLog.txt");
#endif
				this->privOut("\n");
				this->privOut("     MemTrackerLog Link: \n");
				this->privOut("          %s(1) : <Double Click>\n", pString);
				this->privOut("\n");
			}

		public:
			static int LeakCount() noexcept
			{
#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
				_CrtMemState state;
				_CrtMemCheckpoint(&state);

				_CrtMemBlockHeader* pTmp;
				pTmp = reinterpret_cast<MemTrace::_CrtMemBlockHeader*> (state.pBlockHeader);

				int NormBlockLeakCount = 0;

				// Walk to the end of list && see if there is any leaks
				while (pTmp != nullptr)
				{
					if (pTmp->nBlockUse == _NORMAL_BLOCK)
					{
						NormBlockLeakCount++;
					}

					if (pTmp->pBlockHeaderNext == nullptr)
					{
						break;
					}
					pTmp = pTmp->pBlockHeaderNext;
				}
#else
				int NormBlockLeakCount = 0;
#endif

				return NormBlockLeakCount;
			}

			static void ProcessEnd() noexcept
			{
				MemTrace *pTrace = MemTrace::privGetInstance();

				// This is included so you can have one universal include
				std::call_once(pTrace->ProcessEndFlag, [pTrace]()
					{
						FILE* pFile = nullptr;

#ifndef MEMORY_LOGS_DIR
						system("if not exist .\\..\\Logs mkdir .\\..\\Logs");
						fopen_s(&pFile, ".\\..\\Logs\\MemTrackerLog.txt", "w");
#else
						char pString[MemTrace::MemStringSize] = "\"";
						strcat_s(pString, MemTrace::MemStringSize, MEMORY_LOGS_DIR);
						strcat_s(pString, MemTrace::MemStringSize, "Logs\"\0\"");

						char pMkLogDir[MemTrace::MemStringSize] = "if not exist ";
						strcat_s(pMkLogDir, MemTrace::MemStringSize, pString);
						strcat_s(pMkLogDir, MemTrace::MemStringSize, " mkdir ");
						strcat_s(pMkLogDir, MemTrace::MemStringSize, pString);
						system(pMkLogDir);

						char pString2[MemTrace::MemStringSize] = MEMORY_LOGS_DIR;
						strcat_s(pString2, MemTrace::MemStringSize, "Logs\\MemTrackerLog.txt");
						fopen_s(&pFile, pString2, "w");
#endif

						assert(pFile);
						if (pFile != nullptr)
						{
							fprintf(pFile, "\n");
							fprintf(pFile, "****************************************\n");
							fprintf(pFile, "\n");
							fprintf(pFile, "  MemTrackerLog \n");
							fprintf(pFile, "\n");
							fprintf(pFile, "       %s  \n", __DATE__);
							fprintf(pFile, "       %s  \n", __TIME__);
							fprintf(pFile, "\n");
							fprintf(pFile, "****************************************\n");
							fprintf(pFile, "\n");
							fprintf(pFile, "External Libs: \n");
							fprintf(pFile, "\n");
						}
						_CrtMemState state;
						_CrtMemCheckpoint(&state);

						pTrace->privOut("\n");
						_CrtMemBlockHeader *pTmp;
						pTmp = reinterpret_cast<MemTrace::_CrtMemBlockHeader *> (state.pBlockHeader);

						unsigned int i = 0;

						size_t appMaxIndex = 0;
						size_t appMaxSize = 0;
						size_t appCount = 0;
						size_t appTotalSize = 0;
						_CrtMemBlockHeader *pAppMaxTmp = nullptr;

						size_t extMaxIndex = 0;
						size_t extMaxSize = 0;
						size_t extCount = 0;
						size_t extTotalSize = 0;
						_CrtMemBlockHeader *pExtMaxTmp = nullptr;

						int NormBlockLeakCount = 0;

						// Walk to the end of list && see if there is any leaks
						while (pTmp != nullptr)
						{
							if (pTmp->nBlockUse == _NORMAL_BLOCK)
							{
								NormBlockLeakCount++;
							}

							if (pTmp->pBlockHeaderNext == nullptr)
							{
								break;
							}
							pTmp = pTmp->pBlockHeaderNext;
						}
						
						if (NormBlockLeakCount > 0)
						{
							pTrace->privOut("------------------------------------------------------\n");
							pTrace->privOut(">>>   Memory Tracking: Leaks detected              <<<\n");
							pTrace->privOut(">>>   Double click on file string to Leak location <<<\n");
							pTrace->privOut("------------------------------------------------------\n");
							pTrace->privOut("\n");

							while (pTmp != nullptr)
							{
								if (pTmp->nBlockUse == _NORMAL_BLOCK)
								{
									// treat allocation with/without names as different groups
									if (pTmp->szFileName == nullptr)
									{
										extTotalSize += pTmp->nDataSize;
										extCount++;
										if (extMaxSize < pTmp->nDataSize)
										{
											extMaxSize = pTmp->nDataSize;
											extMaxIndex = i;
											pExtMaxTmp = pTmp;
										}
										if (pFile != nullptr)
										{
											fprintf(pFile, "Leak(%d)  %d bytes ref:%d \n", (int)i, (int)pTmp->nDataSize, (int)pTmp->lRequest);
										}
									}
									else
									{
										appTotalSize += pTmp->nDataSize;
										appCount++;
										if (appMaxSize < pTmp->nDataSize)
										{
											appMaxSize = pTmp->nDataSize;
											appMaxIndex = i;
											pAppMaxTmp = pTmp;
										}

										pTrace->privOut("Leak(%d)  %d bytes ref:%d %s \n", i, pTmp->nDataSize, pTmp->lRequest, pTrace->privStripDir(pTmp->szFileName));
										pTrace->privOut("   %s(%d) : <Double Click> \n", pTmp->szFileName, pTmp->nLine);
										pTrace->privOut("\n");
									}

									i++;
								}
								pTmp = pTmp->pBlockHeaderPrev;
							}

							pTrace->privOut("Memory Tracking statistics \n");
							pTrace->privOut("\n");
							pTrace->privOut("     Application files: \n");
							pTrace->privOut("                num of leaks: %d \n", appCount);
							pTrace->privOut("          total bytes leaked: %d \n", appTotalSize);
							if (pAppMaxTmp != nullptr)
							{
								pTrace->privOut("          largest individual: Leak(%d) size: %d \n", appMaxIndex, pAppMaxTmp->nDataSize);
							}
							pTrace->privOut("\n");
							pTrace->privOut("     External Libs: \n");
							pTrace->privOut("                num of leaks: %d \n", extCount);
							pTrace->privOut("          total bytes leaked: %d \n", extTotalSize);

							if (pExtMaxTmp != nullptr)
							{
								pTrace->privOut("          largest individual: Leak(%d) size: %d \n", extMaxIndex, pExtMaxTmp->nDataSize);
							}
							if (pFile != nullptr)
							{
								fprintf(pFile, "\n");
								fprintf(pFile, "Memory Tracking Stats:\n");
								fprintf(pFile, "\n");
								fprintf(pFile, "     Application files: \n");
								fprintf(pFile, "                num of leaks: %d \n", (int)appCount);
								fprintf(pFile, "          total bytes leaked: %d \n", (int)appTotalSize);
								if (pAppMaxTmp != nullptr)
								{
									fprintf(pFile, "          largest individual: Leak(%d) size: %d \n", (int)appMaxIndex, (int)pAppMaxTmp->nDataSize);
								}
								fprintf(pFile, "\n");
								fprintf(pFile, "     External Libs: \n");
								fprintf(pFile, "                num of leaks: %d \n", (int)extCount);
								fprintf(pFile, "          total bytes leaked: %d \n", (int)extTotalSize);

								if (pExtMaxTmp != nullptr)
								{
									fprintf(pFile, "          largest individual: Leak(%d) size: %d \n", (int)extMaxIndex, (int)pExtMaxTmp->nDataSize);
								}

								fprintf(pFile, "\n");
								fprintf(pFile, "CRT Debug Stats: \n");
								fprintf(pFile, "\n");
								fprintf(pFile, "    %d bytes in %d Free Blocks \n", (int)state.lSizes[_FREE_BLOCK], (int)state.lCounts[_FREE_BLOCK]);
								fprintf(pFile, "    %d bytes in %d Normal Blocks \n", (int)state.lSizes[_NORMAL_BLOCK], (int)state.lCounts[_NORMAL_BLOCK]);
								fprintf(pFile, "    %d bytes in %d CRT Blocks \n", (int)state.lSizes[_CRT_BLOCK], (int)state.lCounts[_CRT_BLOCK]);
								fprintf(pFile, "    %d bytes in %d Ignore Blocks \n", (int)state.lSizes[_IGNORE_BLOCK], (int)state.lCounts[_IGNORE_BLOCK]);
								fprintf(pFile, "    %d bytes in %d Client Blocks \n", (int)state.lSizes[_CLIENT_BLOCK], (int)state.lCounts[_CLIENT_BLOCK]);
							}
							pTrace->privDisplayLogLink();
						
						}

						//pTrace->privOut("\n");
						pTrace->privOut("--------------------------------\n");
						//printf("\n");
						printf("--------------------------------\n");
						if (appCount)
						{
							pTrace->privOut(">>> Memory Tracking: FAIL <<<<<<\n");
							printf(">>> Memory Tracking: FAIL <<<<<<\n");
						}
						else 
						{
							pTrace->privOut("    Memory Tracking: passed \n");
							printf("    Memory Tracking: passed \n");
						}
						pTrace->privOut("--------------------------------\n");
						pTrace->privOut("    Memory Tracking: end()      \n");
						pTrace->privOut("--------------------------------\n");
						printf("--------------------------------\n");
						printf("    Memory Tracking: end()      \n");
						printf("--------------------------------\n");
						printf("\n");

						//_CrtMemDumpStatistics(&state);

						if (pFile != nullptr)
						{
							fclose(pFile);
						}

					}
				);
			};

			static void ProcessBegin() noexcept
			{
				MemTrace *pTrace = MemTrace::privGetInstance();

				// This is included so you can have one universal include
				std::call_once(pTrace->ProcessBeginFlag, [pTrace]()
					{
						pTrace->privOut("\n");
						pTrace->privOut("****************************************\n");
						pTrace->privOut("**      Framework: %s               **\n", FRAMEWORK_VER);
						pTrace->privOut("**   C++ Compiler: %d          **\n", _MSC_FULL_VER);
						pTrace->privOut("**  Tools Version: %s        **\n", TOOLS_VERSION);
						pTrace->privOut("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM);
						pTrace->privOut("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING);
						pTrace->privOut("**           Mode: %s        **\n", BUILD_CONFIG_MODE);
						pTrace->privOut("****************************************\n");
						pTrace->privOut("\n");
						printf("\n");
						printf("****************************************\n");
						printf("**      Framework: %s               **\n", FRAMEWORK_VER);
						printf("**   C++ Compiler: %d          **\n", _MSC_FULL_VER);
						printf("**  Tools Version: %s        **\n", TOOLS_VERSION);
						printf("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM);
						printf("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING);
						printf("**           Mode: %s        **\n", BUILD_CONFIG_MODE);
						printf("****************************************\n");
						printf("\n");
						pTrace->privOut("--------------------------------\n");
						pTrace->privOut("    Memory Tracking: start()    \n");
						pTrace->privOut("--------------------------------\n");
						pTrace->privOut("\n");
						printf("--------------------------------\n");
						printf("    Memory Tracking: start()    \n");
						printf("--------------------------------\n");
						printf("\n");
					}
				);
			}

			static void ProcessBegin_Release() 
			{
				MemTrace *pTrace = MemTrace::privGetInstance();

				// This is included so you can have one universal include
				std::call_once(pTrace->ProcessBeginFlag, [pTrace]()
				{
					pTrace->privOut("\n"); 
					pTrace->privOut("****************************************\n"); 
					pTrace->privOut("**      Framework: %s               **\n", FRAMEWORK_VER); 
					pTrace->privOut("**   C++ Compiler: %d          **\n", _MSC_FULL_VER); 
					pTrace->privOut("**  Tools Version: %s        **\n", TOOLS_VERSION);
					pTrace->privOut("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM); 
					pTrace->privOut("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING); 
					pTrace->privOut("**           Mode: %s        **\n", BUILD_CONFIG_MODE);
					pTrace->privOut("****************************************\n"); 
					pTrace->privOut("\n"); 
					printf("\n"); 
					printf("****************************************\n"); 
					printf("**      Framework: %s               **\n", FRAMEWORK_VER); 
					printf("**   C++ Compiler: %d          **\n", _MSC_FULL_VER); 
					printf("**  Tools Version: %s        **\n", TOOLS_VERSION);
					printf("**    Windows SDK: %s       **\n", WINDOWS_TARGET_PLATFORM); 
					printf("**   Mem Tracking: %s   **\n", MEMORY_TRACKING_ENABLE_STRING); 
					printf("**           Mode: %s        **\n", BUILD_CONFIG_MODE);
					printf("****************************************\n"); 
					printf("\n");
				}
				);
			}

			std::once_flag ProcessBeginFlag;
			std::once_flag ProcessEndFlag;

		private:
			static MemTrace *privGetInstance() noexcept
			{
				// This is where its actually stored (BSS section)
				static MemTrace helper;
				return &helper;
			}
			char privBuff[MemTraceBuffSize];
			std::mutex mtx;
		};

		#if defined(_DEBUG) && defined(MEM_TRACKER_ENABLED)
			#define _CRTDBG_MAP_ALLOC  
			#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#else
			#pragma warning( push )
				#pragma warning( disable : 4820 )
				#pragma warning( disable : 4577 )
				#pragma warning( disable : 4987 )
				#include <new>
			#pragma warning( pop )
		#endif

		#ifdef MEM_TRACKER_ENABLED
			#ifdef _DEBUG
				#define MEM_TRACKER_PROCESS_BEGIN	\
						MemTrace::ProcessBegin();

				#define MEM_TRACKER_PROCESS_END \
						MemTrace::ProcessEnd();
			#else
				#define MEM_TRACKER_PROCESS_BEGIN		\
						MemTrace::ProcessBegin_Release();

				#define MEM_TRACKER_PROCESS_END

			#endif
		#else
			#define MEM_TRACKER_PROCESS_BEGIN		\
						MemTrace::ProcessBegin_Release();
		
			#define MEM_TRACKER_PROCESS_END

		#endif

		// MEM_TRACKER_BEGIN:
		static struct _StaticMem 
		{ 
			_StaticMem() noexcept
			{ 
				MEM_TRACKER_PROCESS_BEGIN 
			} 
			~_StaticMem() 
			{ 
				MEM_TRACKER_PROCESS_END 
			} 
			_StaticMem(const _StaticMem &) = delete; 
			_StaticMem(_StaticMem &&) = delete; 
			_StaticMem &operator = (const _StaticMem &) = delete; 
			_StaticMem &operator = (_StaticMem &&) = delete; 
		} _StaticMemInst; 


#endif  MEM_TRACKER_H



#ifndef UNIT_TEST_CPP_H
#define UNIT_TEST_CPP_H

		class UnitTrace
		{
		private:
			static const unsigned int UnitTraceBuffSize = 256;

		public:
			// displays a printf to the output window
			static void out(const char * const fmt, ...)
			{
				UnitTrace *pTrace = UnitTrace::privGetInstance();
				assert(pTrace);

				pTrace->mtx.lock();

				va_list args;

				#pragma warning( push )
					#pragma warning( disable : 26492 )
					#pragma warning( disable : 26481 )
					va_start(args, fmt);
				#pragma warning( pop )

				vsprintf_s(&pTrace->privBuff[0], UnitTraceBuffSize, fmt, args);
				OutputDebugString(&pTrace->privBuff[0]);

				//va_end(args);
				args = static_cast<va_list> (nullptr);

				pTrace->mtx.unlock();
			}

			static void out2(const char* const fmt, ...)
			{
				UnitTrace* pTrace = UnitTrace::privGetInstance();
				assert(pTrace);

				pTrace->mtx.lock();

				va_list args;

				#pragma warning( push )
					#pragma warning( disable : 26492 )
					#pragma warning( disable : 26481 )
				va_start(args, fmt);
				#pragma warning( pop )

				vsprintf_s(&pTrace->privBuff[0], UnitTraceBuffSize, fmt, args);
				vprintf(fmt, args);
				OutputDebugString(&pTrace->privBuff[0]);

				//va_end(args);
				args = static_cast<va_list> (nullptr);

				pTrace->mtx.unlock();
			}

			// Big Six
			UnitTrace() noexcept
			{
				memset(&privBuff[0], 0x0, UnitTraceBuffSize);
			}
			UnitTrace(const UnitTrace &) = delete;
			UnitTrace(UnitTrace &&) = delete;
			UnitTrace & operator = (const UnitTrace &) = delete;
			UnitTrace & operator = (UnitTrace &&) = delete;
			~UnitTrace() = default;

		private:
			static UnitTrace *privGetInstance() noexcept
			{
				// This is where its actually stored (BSS section)
				static UnitTrace helper;
				return &helper;
			}
			char privBuff[UnitTraceBuffSize];
			std::mutex mtx;
		};
		class UnitUtility
		{
		public:
			static bool AreEqual(float a, float b, float epsilon = 0.001f) noexcept
			{
				return (fabs(a - b) < epsilon);
			}

			static bool AreEqual(double a, double b, double epsilon = 0.001f) noexcept
			{
				return (fabs(a - b) < epsilon);
			}
		};
		struct UnitStats
		{
			// Big six
			UnitStats() noexcept
				:testCount(0),
				testPass(0),
				testFail(0),
				indvAsserts(0)
			{
			}
			UnitStats(const UnitStats &) = default;
			UnitStats(UnitStats &&) = default;
			UnitStats & operator = (const UnitStats &) = default;
			UnitStats & operator = (UnitStats &&) = default;
			~UnitStats() = default;

			// data: ------------------
			int testCount;
			int testPass;
			int testFail;
			int indvAsserts;
		};
		struct UnitData
		{
			// Big six
			UnitData() noexcept
				: pMemberName(nullptr),
				pSourceFilePath(nullptr),
				sourceLineNumber(0),
				result(false),
				pad0(0),
				pad1(0),
				pad2(0)
			{
			}
			UnitData(const UnitData &) = delete;
			UnitData(UnitData &&) = delete;
			UnitData & operator = (const UnitData &) = delete;
			UnitData & operator = (UnitData &&) = delete;
			~UnitData() = default;

			// data: -----------------
			const char *pMemberName;
			const char *pSourceFilePath;
			int sourceLineNumber;
			bool result;
			char pad0;
			char pad1;
			char pad2;
		};
		class UnitSLink
		{
		public:
			// Big Six
			UnitSLink() noexcept
				:_pNext(nullptr)
			{
			}
			UnitSLink(const UnitSLink &) = delete;
			UnitSLink(UnitSLink &&) = delete;
			UnitSLink & operator = (const UnitSLink &) = delete;
			UnitSLink & operator = (UnitSLink &&) = delete;
			virtual ~UnitSLink() = default;

			static void AddToFront(UnitSLink *&pRoot, UnitSLink &rNode) noexcept
			{
				if (pRoot == nullptr)
				{
					pRoot = &rNode;
					assert(rNode._pNext == nullptr);
				}
				else
				{
					UnitSLink *pTmp = pRoot;
					pRoot = &rNode;
					rNode._pNext = pTmp;
				}
			}
			static void AddToEnd(UnitSLink *&pRoot, UnitSLink *pNode) noexcept
			{
				if (nullptr == pRoot)
				{
					pRoot = pNode;

					assert(pNode != nullptr);
					assert(pNode->_pNext == nullptr);
				}
				else
				{
					UnitSLink *pTmpX = pRoot;

					while (pTmpX != nullptr)
					{
						if (pTmpX->_pNext == nullptr)
						{
							// at the end
							pTmpX->_pNext = pNode;
							pNode->_pNext = nullptr;
						}

						pTmpX = pTmpX->_pNext;
					}
				}
			}

		public:
			// Data
			UnitSLink *_pNext;
		};
		class TestRegistry
		{
		public:
			// Big four
			TestRegistry(const TestRegistry &) = delete;
			TestRegistry(TestRegistry &&) = delete;
			TestRegistry & operator = (const TestRegistry &) = delete;
			TestRegistry & operator = (TestRegistry &&) = delete;
			~TestRegistry() = default;

			void AddTest(UnitSLink *pTest) noexcept
			{
				// add to End	
				UnitSLink::AddToEnd(this->_pRoot, pTest);
			}
			UnitStats &GetStats() noexcept
			{
				return this->_UnitStats;
			}
			UnitData &GetData() noexcept
			{
				return this->_UnitData;
			}
			UnitSLink *GetRoot() noexcept
			{
				return this->_pRoot;
			}
			static TestRegistry &GetInstance() noexcept
			{
				static TestRegistry tRegistry;
				return tRegistry;
			}
			static UnitStats Stats() noexcept
			{
				TestRegistry &reg = TestRegistry::GetInstance();
				return reg._UnitStats;
			}

		private:
			TestRegistry() noexcept
			{
				this->_pRoot = nullptr;
			}

			// Data: ------------------------
			UnitData _UnitData;
			UnitStats _UnitStats;
			UnitSLink *_pRoot;
		};
		class Test : public UnitSLink
		{
		public:

			Test(const char * const pTestName) noexcept
				:UnitSLink(),
				pName(pTestName),
				testFunc(nullptr)
			{
				// functional pointer
				this->testFunc = this;

				// register it
				TestRegistry &tR = TestRegistry::GetInstance();
				tR.AddTest(this);
			}

			// Big six
			Test() = delete;
			Test(const Test &) = delete;
			Test(Test &&) = delete;
			Test & operator = (const Test &) = delete;
			Test & operator = (Test &&) = delete;
			~Test() = default;
			virtual void run(UnitData &, UnitStats &) = 0;

			// For Tests with NO Teardowns... do nothing
			virtual void teardown() noexcept
			{
			};

			static void RunTests()
			{
			#ifdef _DEBUG
				UnitTrace::out2("------------------- Testing DEBUG ------------------------\n");

				#ifdef _M_X64
						const char * const mode = "x64 Debug";
				#else
						const char * const mode = "x86 Debug";
				#endif
			#else
				#ifdef _M_X64
						const char * const mode = "x64 Release";
				#else
						const char * const mode = "x86 Release";
				#endif

				#ifdef MR_FAST   // Only used in optimized class
						UnitTrace::out2("------------------- Testing MR_FAST ----------------------\n");
				#else
						UnitTrace::out2("------------------- Testing RELEASE ----------------------\n");
				#endif
			#endif

				UnitTrace::out2("\n");
				TestRegistry &rRegistry = TestRegistry::GetInstance();
				UnitSLink *pTmp = rRegistry.GetRoot();

				UnitStats &unitStats = rRegistry.GetStats();
				UnitData  &unitData = rRegistry.GetData();

				while (pTmp != nullptr)
				{
					unitStats.testCount++;

					// downcast to the test
					Test *pTest = (Test *)(pTmp);

					assert(pTest);
					UnitTrace::out2("Test: %s \n", pTest->pName);

					// Needed to be added - for fencing issues between tests
					// Release rearranges.. and affects timing

					// Forces a Fence... 
					atomic_thread_fence(std::memory_order_acq_rel);

						// run the test
						unitData.result = true;

						assert(pTest->testFunc != nullptr);
						pTest->testFunc->run(unitData, unitStats);

					// Forces a Fence... 
					atomic_thread_fence(std::memory_order_acq_rel);

						// Always call the teardown
						pTest->testFunc->teardown();

					// Forces a Fence... 
					atomic_thread_fence(std::memory_order_acq_rel);


					if (unitData.result == true)
					{
						unitStats.testPass++;
					}
					else
					{
						unitStats.testFail++;
					}

					// next test
					pTmp = pTmp->_pNext;
				}

				if (unitStats.testFail)
				{
					UnitTrace::out2("\n");
				}
				UnitTrace::out2("\n");
				UnitTrace::out2(" (%s) testPass: %d\n", mode, unitStats.testPass);
				UnitTrace::out2(" (%s) testFail: %d\n", mode, unitStats.testFail);
				UnitTrace::out2("\n");
				UnitTrace::out2(" testCount: %d\n", unitStats.testCount);
				UnitTrace::out2("indvChecks: %d\n", unitStats.indvAsserts);
				UnitTrace::out2("\n");
				UnitTrace::out2("-----------------\n");

			}

		public:
			const char * const pName;
			Test *testFunc;
		};

		// a trick to create a c-string
		#define STRING_ME(s) #s

		// Increments total check count
		// Creates a clickable format in the output window for failure
		// Abort test on first fail
		#define CHECK( condition ) \
		{ \
			_UnitStats.indvAsserts++;\
			if( !( condition ) ) \
			{ \
				_UnitData.result = false;  \
				_UnitData.pMemberName = this->pName; \
				_UnitData.pSourceFilePath = __FILE__;  \
				_UnitData.sourceLineNumber = __LINE__; \
				UnitTrace::out2("%s(%d): %s \n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName ); \
				return; \
			} \
			else\
			{\
			}\
		}

		#define CHECK_EQUAL( value1, value2 ) \
		{ \
			_UnitStats.indvAsserts++;\
			if( !( (value1) == (value2) ) ) \
			{ \
				_UnitData.result = false;  \
				_UnitData.pMemberName = this->pName; \
				_UnitData.pSourceFilePath = __FILE__; \
				_UnitData.sourceLineNumber = __LINE__; \
				UnitTrace::out2("%s(%d): %s\n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName  ); \
				return; \
			} \
			else\
			{\
			}\
		}

		// -----------------------------------------------------------------------------
		// Here is the unit test magic
		//
		// 1) it creates a class, <TestName>_Test
		//    it is derived from Test class
		// 2) it defines the constructor and calls the base constructor with the name
		//    it registers the name of the test to a single class (TestRegistry)
		// 3) It also overloads the Test::run() method with the body of the macro
		//
		// -----------------------------------------------------------------------------

		#define TEST_END }

		#define TEST( TestName, GroupName, TestFlagEnable ) \
		class TestName##_##GroupName##_Test : public Test \
		{ \
			public: \
				TestName##_##GroupName##_Test() noexcept : \
				Test( STRING_ME(TestName##_##GroupName##_Test)) \
				{ \
				}; \
				TestName##_##GroupName##_Test & operator = (const TestName##_##GroupName##_Test &) = delete;\
				TestName##_##GroupName##_Test(const TestName##_##GroupName##_Test &) = delete;\
			\
			void run(UnitData &_UnitData, UnitStats &_UnitStats) override;\
		} TestName##_##GroupName##_instance; \
		\
		void TestName##_##GroupName##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) \
		{ \
			if(!TestFlagEnable) \
			{ \
				_UnitStats.testCount--; \
				_UnitStats.testPass--; \
			} \
			else 

		#define TEST_WITH_TEARDOWN( TestName, GroupName, TestFlagEnable ) \
		class TestName##_##GroupName##_Test : public Test \
		{ \
			public: \
				TestName##_##GroupName##_Test() noexcept : \
				Test( STRING_ME(TestName##_##GroupName##_Test)) \
				{ \
				}; \
				TestName##_##GroupName##_Test & operator = ( const TestName##_##GroupName##_Test & ) = delete;\
				TestName##_##GroupName##_Test( const TestName##_##GroupName##_Test & ) = delete;\
			\
			void run(UnitData &_UnitData, UnitStats &_UnitStats) override;\
			void teardown() noexcept override;\
		} TestName##_##GroupName##_instance; \
		\
		void TestName##_##GroupName##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) \
		{ \
			if (!TestFlagEnable) \
			{ \
				_UnitStats.testCount--; \
				_UnitStats.testPass--; \
			} \
			else


		#define TEST_TEARDOWN( TestName, GroupName ) \
		void TestName##_##GroupName##_Test::teardown() noexcept

#endif  UNIT_TEST_CPP_H

#ifndef PERFORMANCE_TIMER_H
#define PERFORMANCE_TIMER_H

		class PerformanceTimer
		{
		public:
			// big six
			PerformanceTimer() noexcept
				:
				ticTime(),
				tocTime(),
				deltaTime(),
				SecondsPerCycle(0.0f),
				timeSeconds(0.0f)
			{
				this->privInitTimer();
				this->Reset();
			}
			PerformanceTimer(const PerformanceTimer &) = delete;
			PerformanceTimer(PerformanceTimer &&) = delete;
			PerformanceTimer & operator= (const PerformanceTimer &) = delete;
			PerformanceTimer & operator= (PerformanceTimer &&) = delete;
			~PerformanceTimer() = default;

			void Tic() noexcept
			{
				// Forces a Fence... 
				atomic_thread_fence(std::memory_order_acq_rel);

					this->ticTime = this->privGetTimer();

				// Forces a Fence...
				atomic_thread_fence(std::memory_order_acq_rel);
			}
			void Toc() noexcept
			{
				// Forces a Fence... 
				atomic_thread_fence(std::memory_order_acq_rel);

					this->tocTime = this->privGetTimer();
					assert(this->tocTime.QuadPart >= this->ticTime.QuadPart);
					this->deltaTime.QuadPart = this->tocTime.QuadPart - this->ticTime.QuadPart;

				// Forces a Fence... 
				atomic_thread_fence(std::memory_order_acq_rel);
			}
			void Reset() noexcept
			{
				this->ticTime.QuadPart = 0;
				this->tocTime.QuadPart = 0;
				this->deltaTime.QuadPart = 0;
			}
			float TimeInSeconds() noexcept
			{
				float floatTime;
				floatTime = static_cast<float>(this->deltaTime.QuadPart);
				floatTime *= this->SecondsPerCycle;
				return floatTime;
			}

		private:

			void privInitTimer() noexcept
			{
				LARGE_INTEGER Frequency;
				QueryPerformanceFrequency(&Frequency);
				this->SecondsPerCycle = 1.0f / Frequency.QuadPart;
			}
			LARGE_INTEGER privGetTimer() noexcept
			{
				LARGE_INTEGER time;
				QueryPerformanceCounter(&time);
				return time;
			}

			// ------------------------------------------
			// data
			// ------------------------------------------

			LARGE_INTEGER	ticTime;
			LARGE_INTEGER	tocTime;
			LARGE_INTEGER	deltaTime;
			float			SecondsPerCycle;
			float			timeSeconds;
		};

#endif PERFORMANCE_TIMER_H


#ifndef DEBUG_OUTPUT_H
#define DEBUG_OUTPUT_H

		class Trace
		{
		private:
			static const unsigned int TraceBuffSize = 256;

		public:

			// Big six
			Trace() noexcept
			{
				memset(&privBuff[0], 0x0, TraceBuffSize);
			}
			Trace(const Trace &) = delete;
			Trace(Trace &&) = delete;
			Trace & operator = (const Trace &) = delete;
			Trace & operator = (Trace &&) = delete;
			~Trace() = default;

			// displays a printf to the output window
			static void out(const char * const fmt, ...) 
			{
				Trace *pTrace = Trace::privGetInstance();
				assert(pTrace);

				pTrace->mtx.lock();

				va_list args;

				#pragma warning( push )
					#pragma warning( disable : 26492 )
					#pragma warning( disable : 26481 )
					va_start(args, fmt);
				#pragma warning( pop )

				vsprintf_s(&pTrace->privBuff[0], TraceBuffSize, fmt, args);
				OutputDebugString(&pTrace->privBuff[0]);

				//va_end(args);
				args = static_cast<va_list> (nullptr);

				pTrace->mtx.unlock();
			}

			static void out2(const char* const fmt, ...)
			{
				Trace* pTrace = Trace::privGetInstance();
				assert(pTrace);

				pTrace->mtx.lock();

				va_list args;

				#pragma warning( push )
					#pragma warning( disable : 26492 )
					#pragma warning( disable : 26481 )
				va_start(args, fmt);
				#pragma warning( pop )

				vsprintf_s(&pTrace->privBuff[0], TraceBuffSize, fmt, args);
				vprintf(fmt, args);
				OutputDebugString(&pTrace->privBuff[0]);

				//va_end(args);
				args = static_cast<va_list> (nullptr);

				pTrace->mtx.unlock();
			}

		private:
			static Trace *privGetInstance() noexcept
			{
				static Trace helper;
				return &helper;
			}
			char privBuff[TraceBuffSize];
			std::mutex mtx;
		};

#endif DEBUG_OUTPUT_H


#ifndef FILE_IO_H
#define FILE_IO_H


		#ifdef _DEBUG
			#ifdef _M_X64
					const char* const pFileio_mode = "x64_Debug";
			#else
					const char* const pFileio_mode = "x86_Debug";
			#endif
		#else
			#ifdef _M_X64
					const char* const pFileio_mode = "x64_Release";
			#else
					const char* const pFileio_mode = "x86_Release";
			#endif
		#endif

		class FileIO
		{
		public:

			static void Open(const char * const pFirstName, const char * const pLastName) noexcept
			{
				assert(pFirstName);
				assert(pLastName);
				FileIO::privGetInstance()->privOpen(pFirstName, pLastName);
			}
			static void Close() noexcept
			{
				FileIO::privGetInstance()->privClose();
			}
			static FILE *GetHandle() noexcept
			{
				return FileIO::privGetInstance()->privGetHandle();
			}

		private:

			void privOpen(const char * const pFirstName, const char * const pLastName) noexcept
			{
				system("if not exist .\\..\\Logs mkdir .\\..\\Logs");

				const char * const pFile_extension = ".txt";
				const char * const pFile_io_path = ".\\..\\Logs\\";

				constexpr int length = 256;

				char pFileName[length] = { 0 };
				assert(pFileName);

				errno_t fileError(0);

				// wash the name to 0
				memset(&pFileName[0], 0, length);

				// is there enough of space?
				assert( (strlen(pFile_io_path) 
						+ strlen(pFirstName) 
						+ strlen(pLastName) 
						+ strlen("-") 
						+ strlen(pFileio_mode) 
						+ strlen(pFile_extension) 
						+ strlen("/0") ) < length);

				strcat_s(&pFileName[0], length, pFile_io_path);
				strcat_s(&pFileName[0], length, pFirstName);
				strcat_s(&pFileName[0], length, pLastName);
				strcat_s(&pFileName[0], length, "_");
				strcat_s(&pFileName[0], length, pFileio_mode);
				strcat_s(&pFileName[0], length, pFile_extension);


				fileError = fopen_s(&pFileHandle, &pFileName[0], "wt");
				assert(pFileHandle);
				if(pFileHandle != nullptr)
				{
					fprintf(this->pFileHandle, "-------------------------------------------------\n");
					fprintf(this->pFileHandle, "\n");
					fprintf(this->pFileHandle, "Log File \n");
					fprintf(this->pFileHandle, "\n");
					fprintf(this->pFileHandle, "Name: %s %s\n", pFirstName, pLastName);
					fprintf(this->pFileHandle, "Mode: %s\n", pFileio_mode);
					fprintf(this->pFileHandle, "\n");
					fprintf(this->pFileHandle, "-------------------------------------------------\n");
					fprintf(this->pFileHandle, "\n");
				}
			}
			FILE *privGetHandle() noexcept
			{
				assert(pFileHandle);
				return this->pFileHandle;
			}
			static FileIO *privGetInstance() noexcept
			{
				static FileIO instance;
				return &instance;
			}
			void privClose() noexcept
			{
				errno_t fileError(0);
				assert(pFileHandle);

				fileError = fflush(this->pFileHandle);
				assert(!fileError);

				fileError = fclose(this->pFileHandle);
				this->pFileHandle = nullptr;
				assert(!fileError);
			}

			// big six  
			constexpr FileIO() noexcept
				:pFileHandle(nullptr)
			{

			};
			FileIO(const FileIO &) = delete;
			FileIO(FileIO &&) = delete;
			FileIO & operator=(const FileIO &) = delete;
			FileIO & operator=(FileIO &&) = delete;
			~FileIO()
			{
				if (nullptr != this->pFileHandle)
				{
					this->privClose();
				}
			};

			// ------------------------------------------
			// data
			// ------------------------------------------

			FILE *pFileHandle;
		};

#endif FILE_IO_H



#ifndef ALIGN_16_H
#define ALIGN_16_H

		#define ALIGN_UNUSED_VAR(x) (void(x))

		#pragma push_macro("new")
		#undef new

		class Align16
		{
		public:

			// Placement new for align16
			void* operator new(size_t, void *p) noexcept
			{
				ALIGN_UNUSED_VAR(p);
				return p;
			}

			void *operator new(size_t size) noexcept
			{
				void *p = _aligned_malloc(size, 16);
				return p;
			}

			void operator delete(void *p)
			{
				_aligned_free(p);
			}

			void *operator new[](size_t size) noexcept
			{
				void *p = _aligned_malloc(size, 16);
				return p;
			}

			void operator delete[](void *p)
			{
				_aligned_free(p);
			}

			void *operator new(size_t      size,
								int         _BlockUse,
								char const* _FileName,
								int         _LineNumber) noexcept
			{
				ALIGN_UNUSED_VAR(_BlockUse);
				ALIGN_UNUSED_VAR(_FileName);
				ALIGN_UNUSED_VAR(_LineNumber);

				void *p = _aligned_malloc_dbg(size, 16, _FileName, _LineNumber);
				return p;
			}

			void operator delete(void        *p,
									int         _BlockUse,
									char const* _FileName,
									int         _LineNumber)
			{
				ALIGN_UNUSED_VAR(_BlockUse);
				ALIGN_UNUSED_VAR(_FileName);
				ALIGN_UNUSED_VAR(_LineNumber);
				_aligned_free_dbg(p);
			}

			void *operator new[](size_t        size,
									int         _BlockUse,
									char const* _FileName,
									int         _LineNumber)  noexcept
			{
				ALIGN_UNUSED_VAR(_BlockUse);
				ALIGN_UNUSED_VAR(_FileName);
				ALIGN_UNUSED_VAR(_LineNumber);

				void *p = _aligned_malloc_dbg(size, 16, _FileName, _LineNumber);
				return p;
			}

			void operator delete[](void      *p,
									int         _BlockUse,
									char const* _FileName,
									int         _LineNumber)
			{
				ALIGN_UNUSED_VAR(_BlockUse);
				ALIGN_UNUSED_VAR(_FileName);
				ALIGN_UNUSED_VAR(_LineNumber);
				_aligned_free_dbg(p);
			}

		};

		#pragma pop_macro("new")

#endif //ALIGN_16_H


#endif //FRAMEWORK_H

// ---  End of File ---------------
