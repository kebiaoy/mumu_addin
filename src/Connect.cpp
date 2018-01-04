// Connect.cpp : Implementation of CConnect
#include "stdafx.h"
#include "AddIn.h"
#include "Connect.h"
#include "mu_strutil.h"
#include "mu_solution.h"
#include "mu_pathutil.h"
#include "mu_project.h"
#include "mu_generator.h"


extern CAddInModule _AtlModule;

// CConnect
STDMETHODIMP CConnect::OnConnection(IDispatch *pApplication, ext_ConnectMode ConnectMode, IDispatch *pAddInInst, SAFEARRAY ** /*custom*/)
{
	HRESULT hr = S_OK;
	pApplication->QueryInterface(__uuidof(DTE2), (LPVOID*)&m_pDTE);
	pAddInInst->QueryInterface(__uuidof(AddIn), (LPVOID*)&m_pAddInInstance);
	if (ConnectMode == ext_ConnectMode::ext_cm_Startup)
	{
		CComPtr<EnvDTE::Events> pEvents;
		IfFailGo(m_pDTE->get_Events(&pEvents));

		// Get DTE.BuildEvents interface
		IfFailGo(pEvents->get_DebuggerEvents((EnvDTE::_DebuggerEvents**)&m_pDebugEvents));
		IfFailGo(pEvents->get_BuildEvents((EnvDTE::_BuildEvents**)&m_pBuildEvents));

		// Sink the BuildEvents
		IfFailGo(DebuggerEventImpl::DispEventAdvise((IUnknown*)m_pDebugEvents.p));
		IfFailGo(BuildEventImpl::DispEventAdvise((IUnknown*)m_pBuildEvents.p));
	}
Error:
	return hr;
}

STDMETHODIMP CConnect::OnDisconnection(ext_DisconnectMode /*RemoveMode*/, SAFEARRAY ** /*custom*/)
{
	// 	m_pDTE = NULL;
	// 	m_pAddInInstance = NULL;
	return S_OK;
}

STDMETHODIMP CConnect::OnAddInsUpdate(SAFEARRAY ** /*custom*/)
{
	return S_OK;
}

STDMETHODIMP CConnect::OnStartupComplete(SAFEARRAY ** /*custom*/)
{
	return S_OK;
}

STDMETHODIMP CConnect::OnBeginShutdown(SAFEARRAY ** /*custom*/)
{
	return S_OK;
}

STDMETHODIMP CConnect::QueryStatus(BSTR bstrCmdName, vsCommandStatusTextWanted NeededText, vsCommandStatus *pStatusOption, VARIANT *pvarCommandText)
{
	return S_OK;
}

STDMETHODIMP CConnect::Exec(BSTR bstrCmdName, vsCommandExecOption ExecuteOption, VARIANT * /*pvarVariantIn*/, VARIANT * /*pvarVariantOut*/, VARIANT_BOOL *pvbHandled)
{
	return S_OK;
}

void __stdcall CConnect::OnEnterRunMode(EnvDTE::dbgEventReason Reason)
{

}

void __stdcall CConnect::OnEnterDesignMode(EnvDTE::dbgEventReason Reason)
{

}

void __stdcall CConnect::OnEnterBreakMode(EnvDTE::dbgEventReason Reason, EnvDTE::dbgExecutionAction* ExecutionAction)
{

}

void __stdcall CConnect::OnExceptionThrown(BSTR ExceptionType, BSTR Name, long Code, BSTR Description, EnvDTE::dbgExceptionAction* ExceptionAction)
{

}

void __stdcall CConnect::OnExceptionNotHandled(BSTR ExceptionType, BSTR Name, long Code, BSTR Description, EnvDTE::dbgExceptionAction* ExceptionAction)
{

}

void __stdcall CConnect::OnContextChanged(EnvDTE::Process* NewProcess, EnvDTE::Program* NewProgram, EnvDTE::Thread* NewThread, EnvDTE::StackFrame* NewStackFrame)
{

}

void __stdcall CConnect::OnBuildBegin(EnvDTE::vsBuildScope Scope, EnvDTE::vsBuildAction Action)
{
	
}

void __stdcall CConnect::OnBuildDone(EnvDTE::vsBuildScope Scope, EnvDTE::vsBuildAction Action)
{

}

void __stdcall CConnect::OnBuildProjConfigBegin(BSTR sProject, BSTR ProjectConfig, BSTR Platform, BSTR SolutionConfig)
{
	EnvDTE::_Solution* pSolutionImpl;
	m_pDTE->get_Solution(&pSolutionImpl);
	mu_solution_ptr pSolution = mu_solution_ptr(new mu_solution(pSolutionImpl));
	mu_project_ptr pProject = pSolution->get_project(mu_pathutil::get_filename(mu_strutil::w2s(CP_UTF8, sProject)));
	mu_generator_ptr pGenerator = mu_generator_ptr(new mu_generator(pProject));
	pGenerator->generate();
}

void __stdcall CConnect::OnBuildProjConfigDone(BSTR Project, BSTR ProjectConfig, BSTR Platform, BSTR SolutionConfig, VARIANT_BOOL Success)
{

}
