#pragma once



class __declspec(dllexport) ServiceManager
{
private:
	SC_HANDLE _scManager;
public:
	ServiceManager();
	~ServiceManager();
	void Initialize();

private:
	void OnError();
};

