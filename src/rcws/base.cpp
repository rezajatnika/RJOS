
#include <string>
#include <iostream>
#include <vector>
#include "base.hpp"
#include "handler.h"


using namespace std;


namespace RCWS {
	struct RemoteWeaponSystemServer::RcwsData_t {
		double correction;
		double traverse;
		double elevation;
		double speed;
	};

	RemoteWeaponSystemServer::RemoteWeaponSystemServer()
	{
		_FiringMode = 0;
		_priv = new RcwsData_t;
	}

	RemoteWeaponSystemServer::~RemoteWeaponSystemServer()
	{
		delete _priv;
		_priv = nullptr;
	}

	int RemoteWeaponSystemServer::SetAzimuth(float setpoint) {
		cout << "received " << __FUNCTION__ << ". " << setpoint << endl;
		
		return handler_barrel_set_azimuth(setpoint);
	}

	int RemoteWeaponSystemServer::SetElevation(float setpoint) {
		cout << "received " << __FUNCTION__ << ". " << setpoint << endl;

		return handler_barrel_set_elevation(setpoint);
	}

	bool RemoteWeaponSystemServer::HaltCurrentAction(bool force) {
		cout << "received " << __FUNCTION__ << ". " << force << endl;

		return false;
	}

	float RemoteWeaponSystemServer::SetManualControlRate(float speed) {
		cout << "received " << __FUNCTION__ << ".  " << speed << endl;

		_priv->speed = speed;

		return _priv->speed;
	}

	int RemoteWeaponSystemServer::DoManualControlAzimuth(int dir) {
		cout << "received " << __FUNCTION__ << ".  " << dir << endl;

		return handler_barrel_manual_ctrl_azimuth(dir);
	}

	int RemoteWeaponSystemServer::DoManualControlElevation(int dir) {
		cout << "received " << __FUNCTION__  << ".  " << dir << endl;

		return handler_barrel_manual_ctrl_elevation(dir);
	}

	int RemoteWeaponSystemServer::GetBarrelStatus(int num) {
		cout << "received " << __FUNCTION__ << ".  " << num << endl;
		return 1;
	}

	uint64_t RemoteWeaponSystemServer::GetAllBarrelStatus()
	{
		cout << "received " << __FUNCTION__ << ".  " << endl;
		return 0x0ffff0100681de00ULL;
	}

	/* Array */
	int RemoteWeaponSystemServer::GetRipplePreset() {
		cout << "received " << __FUNCTION__ << ".  " << endl;
		return 1;
	}

	int RemoteWeaponSystemServer::SetSelectedBarrels(uint64_t barrels) {
		cout << "received " << __FUNCTION__ << ".  " << barrels << endl;
		return 1;
	}

	int RemoteWeaponSystemServer::ConfirmSelection(bool confirm) {
		cout << "received " << __FUNCTION__ << ".  " << confirm << endl;
		return 1;
	}

	int RemoteWeaponSystemServer::SetFiringMode(int mode) {
		cout << "received " << __FUNCTION__ << ".  " << mode << endl;
		_FiringMode = mode;
		return 1;
	}

	int RemoteWeaponSystemServer::GetFiringMode() {
		cout << "received " << __FUNCTION__ << ".  " << endl;
		
		return _FiringMode;
	}

	int RemoteWeaponSystemServer::GetLauncherStatus() {
		cout << "received " << __FUNCTION__ << ".  " << endl;
		return 1;
	}

	bool RemoteWeaponSystemServer::IsLauncherArmed() {
		cout << "received " << __FUNCTION__ << ".  " << endl;
		return false;
	}


	/* Heading */
	int RemoteWeaponSystemServer::Correction(double north) {
		cout << "received " << __FUNCTION__ << ". north: " << north << endl;

		_priv->correction = north;
		return 0;
	}

}