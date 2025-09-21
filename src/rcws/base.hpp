

#ifndef _INCLUDE_RCWS_BASE_HPP_
#define _INCLUDE_RCWS_BASE_HPP_

//#include <basetsd.h>

namespace RCWS {
	class RemoteWeaponSystemServer {
	public:
		enum LauncherStatusPos {
			LAUNCHERSTATUSPOS_ZERO_POS = 0,
			LAUNCHERSTATUSPOS_DEAD_ANGLE,
			LAUNCHERSTATUSPOS_READY,
			LAUNCHERSTATUSPOS_FILLING,

			LAUNCHERSTATUSPOS_MAX
		};

		enum LauncherStatusArm {
			LAUNCHERSTATUSARM_SAFE = 0,
			LAUNCHERSTATUSARM_ARMED
		};

		RemoteWeaponSystemServer();
		~RemoteWeaponSystemServer();

		int SetAzimuth(float setpoint);
		int SetElevation(float setpoint);
		bool HaltCurrentAction(bool force);
		float SetManualControlRate(float speed);
		virtual int DoManualControlAzimuth(int dir);
		int DoManualControlElevation(int dir);

		/* Additional */
		//int EnableTransportLock(bool lock);
		//int LockLauncherPos(bool lock);

		/* Barrels */
		int GetBarrelStatus(int num);
		uint64_t GetAllBarrelStatus();
		int GetRipplePreset();
		int SetSelectedBarrels(uint64_t barrels);
		int ConfirmSelection(bool confirm);
		int SetFiringMode(int mode);
		int GetFiringMode();
		int GetLauncherStatus();
		bool IsLauncherArmed();

		/* Heading */
		int Correction(double north);


	private:
		struct RcwsData_t;

		RcwsData_t* _priv;
		int _FiringMode;
	};

}



#endif