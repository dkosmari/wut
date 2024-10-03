#pragma once
#include <wut.h>
#include <coreinit/time.h>

/**
 * \defgroup padscore_wpad WPAD
 * \ingroup padscore
 *
 * WPAD is a low-level library under KPAD.
 *
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif

#define WPAD_MAX_IR_DOTS 4
#define WPAD_MAX_PRESSURE_SENSORS 4

typedef struct WPADStatusProController WPADStatusProController;
typedef struct WPADVec2D WPADVec2D;
typedef struct WPADVec3D WPADVec3D;
typedef struct WPADInfo WPADInfo;
typedef struct WPADAddress WPADAddress;
typedef struct WPADiQueueElement WPADiQueueElement;
typedef struct WPADiQueue WPADiQueue;
typedef struct WPADIRDot WPADIRDot;
typedef struct WPADStatus WPADStatus;
typedef struct WPADStatusNunchuk WPADStatusNunchuk;
typedef struct WPADStatusClassic WPADStatusClassic;
typedef struct WPADStatusMotionPlus WPADStatusMotionPlus;
typedef struct WPADStatusBalanceBoard WPADStatusBalanceBoard;
typedef struct WPADStatusTrain WPADStatusTrain;
typedef WPADStatusProController WPADStatusPro;

typedef enum WPADError
{
   WPAD_ERROR_NONE          =  0,
   WPAD_ERROR_NO_CONTROLLER = -1,
   WPAD_ERROR_NOT_READY     = -2,
   WPAD_ERROR_TRANSFER      = -3,
   WPAD_ERROR_INVALID       = -4,
   WPAD_ERROR_PERMISSION    = -5,
   WPAD_ERROR_BROKEN        = -6,
   WPAD_ERROR_QUEUE_EMPTY   = -7,
   WPAD_ERROR_BAD_VALUE     = -8,
   WPAD_ERROR_BAD_CONF      = -9,
} WPADError;

//! Wii Remote channel.
typedef enum WPADChan
{
   WPAD_CHAN_0 = 0,             //!< Channel 0.
   WPAD_CHAN_1 = 1,             //!< Channel 1.
   WPAD_CHAN_2 = 2,             //!< Channel 2.
   WPAD_CHAN_3 = 3,             //!< Channel 3.
   WPAD_CHAN_4 = 4,             //!< Channel 4.
   WPAD_CHAN_5 = 5,             //!< Channel 5.
   WPAD_CHAN_6 = 6,             //!< Channel 6.
} WPADChan;

//! Data format.
typedef enum WPADDataFormat
{
   WPAD_FMT_CORE              = 0, //!< Wii Remote buttons
   WPAD_FMT_CORE_ACC          = 1, //!< Wii Remote buttons and accelerometer
   WPAD_FMT_CORE_ACC_DPD      = 2, //!< Wii Remote  buttons, accelerometer and IR pos
   WPAD_FMT_NUNCHUK           = 3, //!< Wii Remote  buttons, Nunchuk
   WPAD_FMT_NUNCHUK_ACC       = 4, //!< Wii Remote  buttons, accelerometer, Nunchuk
   WPAD_FMT_NUNCHUK_ACC_DPD   = 5, //!< Wii Remote  buttons, accelerometer, IR pos, Nunchuk
   WPAD_FMT_CLASSIC           = 6, //!< Wii Remote  buttons, Classic Controller
   WPAD_FMT_CLASSIC_ACC       = 7, //!< Wii Remote  buttons, accelerometer, Classic Controller
   WPAD_FMT_CLASSIC_ACC_DPD   = 8, //!< Wii Remote  buttons, accelerometer, IR pos, Classic Controller
   WPAD_FMT_CORE_ACC_DPD_FULL = 9, //!< Wii Remote  buttons, accelerometer and IR pos with bounds
   WPAD_FMT_TRAIN             = 10, //!< Wii        Remote, Densha De GO! Shinkansen Controller
   WPAD_FMT_GUITAR            = 11, //!< Guitar Hero Guitar
   WPAD_FMT_BALANCE_BOARD     = 12, //!< Wii Balance Board
   WPAD_FMT_DRUM              = 15, //!< Guitar Hero World Tour Drums
   WPAD_FMT_MPLUS             = 16, //!< Wii Remote buttons, accelerometer, IR pos, Motion Plus gyroscope
   WPAD_FMT_TAIKO             = 17, //!< Wii        Remote, Taiko no Testsujin TaTaCon
   WPAD_FMT_PRO_CONTROLLER    = 22, //!< Wii U Pro Controller
} WPADDataFormat;

//! Extension type.
typedef enum WPADExtensionType
{
   WPAD_EXT_CORE           = 0x00, //!< Wii Remote with no extension.
   WPAD_EXT_NUNCHUK        = 0x01, //!< Nunchuk.
   WPAD_EXT_CLASSIC        = 0x02, //!< Classic Controller.
   WPAD_EXT_MPLUS          = 0x05, //!< Motion Plus.
   WPAD_EXT_MPLUS_NUNCHUK  = 0x06, //!< Motion Plus with Nunchuk.
   WPAD_EXT_MPLUS_CLASSIC  = 0x07, //!< Motion Plus with Classic Controller.
   WPAD_EXT_PRO_CONTROLLER = 0x1f, //!< Pro Controller.
   WPAD_EXT_DEV_NOT_FOUND  = 0xfd, //!< No controller found.
   WPAD_EXT_UNKNOWN        = 0xff, //!< Extension unknown.
} WPADExtensionType;

//! Wii Remote buttons.
typedef enum WPADButton
{
   WPAD_BUTTON_LEFT  = 0x0001,  //!< The left button of the D-pad.
   WPAD_BUTTON_RIGHT = 0x0002,  //!< The right button of the D-pad.
   WPAD_BUTTON_DOWN  = 0x0004,  //!< The down button of the D-pad.
   WPAD_BUTTON_UP    = 0x0008,  //!< The up button of the D-pad.
   WPAD_BUTTON_PLUS  = 0x0010,  //!< The + button.
   WPAD_BUTTON_2     = 0x0100,  //!< The 2 button.
   WPAD_BUTTON_1     = 0x0200,  //!< The 1 button.
   WPAD_BUTTON_B     = 0x0400,  //!< The B button.
   WPAD_BUTTON_A     = 0x0800,  //!< The A button.
   WPAD_BUTTON_MINUS = 0x1000,  //!< The - button.
   WPAD_BUTTON_Z     = 0x2000,  //!< The Z button on the Nunchuk extension.
   WPAD_BUTTON_C     = 0x4000,  //!< The C button on the Nunchuk extension.
   WPAD_BUTTON_HOME  = 0x8000,  //!< The HOME button.
} WPADButton;

//! Nunchuk buttons.
typedef enum WPADNunchukButton
{
   WPAD_NUNCHUK_STICK_EMULATION_LEFT  = 0x0001, //!< The emulated left button on the Nunchuk stick or the left button of the D-pad on the Wii Remote.
   WPAD_NUNCHUK_STICK_EMULATION_RIGHT = 0x0002, //!< The emulated right button on the Nunchuk stick or the right button of the D-pad on the Wii Remote.
   WPAD_NUNCHUK_STICK_EMULATION_DOWN  = 0x0004, //!< The emulated down button on the Nunchuk stick or the down button of the D-pad on the Wii Remote.
   WPAD_NUNCHUK_STICK_EMULATION_UP    = 0x0008, //!< The emulated up button on the Nunchuk stick or the up button of the D-pad on the Wii Remote.
   WPAD_NUNCHUK_BUTTON_Z              = 0x2000, //!< The Z button.
   WPAD_NUNCHUK_BUTTON_C              = 0x4000, //!< The C button.
} WPADNunchukButton;

//! Classic Controller buttons.
typedef enum WPADClassicButton
{
   WPAD_CLASSIC_BUTTON_UP               = 0x00000001, //!< The up button of the D-pad.
   WPAD_CLASSIC_BUTTON_LEFT             = 0x00000002, //!< The left button of the D-pad.
   WPAD_CLASSIC_BUTTON_ZR               = 0x00000004, //!< The ZR button.
   WPAD_CLASSIC_BUTTON_X                = 0x00000008, //!< The X button.
   WPAD_CLASSIC_BUTTON_A                = 0x00000010, //!< The A button.
   WPAD_CLASSIC_BUTTON_Y                = 0x00000020, //!< The Y button.
   WPAD_CLASSIC_BUTTON_B                = 0x00000040, //!< The B button.
   WPAD_CLASSIC_BUTTON_ZL               = 0x00000080, //!< The ZL button.
   WPAD_CLASSIC_BUTTON_R                = 0x00000200, //!< The R button.
   WPAD_CLASSIC_BUTTON_PLUS             = 0x00000400, //!< The + button.
   WPAD_CLASSIC_BUTTON_HOME             = 0x00000800, //!< The HOME button.
   WPAD_CLASSIC_BUTTON_MINUS            = 0x00001000, //!< The - button.
   WPAD_CLASSIC_BUTTON_L                = 0x00002000, //!< The L button.
   WPAD_CLASSIC_BUTTON_DOWN             = 0x00004000, //!< The down button of the D-pad.
   WPAD_CLASSIC_BUTTON_RIGHT            = 0x00008000, //!< The right button of the D-pad.
   WPAD_CLASSIC_STICK_L_EMULATION_LEFT  = 0x00010000, //!< The emulated left button on the left stick.
   WPAD_CLASSIC_STICK_L_EMULATION_RIGHT = 0x00020000, //!< The emulated right button on the left stick.
   WPAD_CLASSIC_STICK_L_EMULATION_DOWN  = 0x00040000, //!< The emulated down button on the left stick.
   WPAD_CLASSIC_STICK_L_EMULATION_UP    = 0x00080000, //!< The emulated up button on the left stick.
   WPAD_CLASSIC_STICK_R_EMULATION_LEFT  = 0x00100000, //!< The emulated left button on the right stick.
   WPAD_CLASSIC_STICK_R_EMULATION_RIGHT = 0x00200000, //!< The emulated right button on the right stick.
   WPAD_CLASSIC_STICK_R_EMULATION_DOWN  = 0x00400000, //!< The emulated down button on the right stick.
   WPAD_CLASSIC_STICK_R_EMULATION_UP    = 0x00800000, //!< The emulated up button on the right stick.
} WPADClassicButton;

//! Pro Controller buttons.
typedef enum WPADProButton
{
   WPAD_PRO_BUTTON_UP               = 0x00000001, //!< The up button of the D-pad.
   WPAD_PRO_BUTTON_LEFT             = 0x00000002, //!< The left button of the D-pad.
   WPAD_PRO_TRIGGER_ZR              = 0x00000004, //!< The ZR button.
   WPAD_PRO_BUTTON_ZR               = WPAD_PRO_TRIGGER_ZR, //!< The ZR button.
   WPAD_PRO_BUTTON_X                = 0x00000008, //!< The X button.
   WPAD_PRO_BUTTON_A                = 0x00000010, //!< The A button.
   WPAD_PRO_BUTTON_Y                = 0x00000020, //!< The Y button.
   WPAD_PRO_BUTTON_B                = 0x00000040, //!< The B button.
   WPAD_PRO_TRIGGER_ZL              = 0x00000080, //!< The ZL button.
   WPAD_PRO_BUTTON_ZL               = WPAD_PRO_TRIGGER_ZL, //!< The ZL button.
   WPAD_PRO_RESERVED                = 0x00000100, //!< Reserved.
   WPAD_PRO_TRIGGER_R               = 0x00000200, //!< The right trigger button.
   WPAD_PRO_BUTTON_R                = WPAD_PRO_TRIGGER_R, //!< The R button.
   WPAD_PRO_BUTTON_PLUS             = 0x00000400, //!< The + button.
   WPAD_PRO_BUTTON_HOME             = 0x00000800, //!< The HOME button.
   WPAD_PRO_BUTTON_MINUS            = 0x00001000, //!< The - button.
   WPAD_PRO_TRIGGER_L               = 0x00002000, //!< The left trigger button.
   WPAD_PRO_BUTTON_L                = WPAD_PRO_TRIGGER_L, //!< The L button.
   WPAD_PRO_BUTTON_DOWN             = 0x00004000, //!< The down button of the D-pad.
   WPAD_PRO_BUTTON_RIGHT            = 0x00008000, //!< The right button of the D-pad.
   WPAD_PRO_BUTTON_STICK_R          = 0x00010000, //!< The right stick button.
   WPAD_PRO_BUTTON_STICK_L          = 0x00020000, //!< The left stick button.
   WPAD_PRO_STICK_L_EMULATION_UP    = 0x00200000, //!< The emulated up button on the left stick.
   WPAD_PRO_STICK_L_EMULATION_DOWN  = 0x00100000, //!< The emulated down button on the left stick.
   WPAD_PRO_STICK_L_EMULATION_LEFT  = 0x00040000, //!< The emulated left button on the left stick.
   WPAD_PRO_STICK_L_EMULATION_RIGHT = 0x00080000, //!< The emulated right button on the left stick.
   WPAD_PRO_STICK_R_EMULATION_UP    = 0x02000000, //!< The emulated up button on the right stick.
   WPAD_PRO_STICK_R_EMULATION_DOWN  = 0x01000000, //!< The emulated down button on the right stick.
   WPAD_PRO_STICK_R_EMULATION_LEFT  = 0x00400000, //!< The emulated left button on the right stick.
   WPAD_PRO_STICK_R_EMULATION_RIGHT = 0x00800000, //!< The emulated right button on the right stick.
} WPADProButton;

//! WPAD Led flags
typedef enum WPADLed
{
   WPAD_LED_ONE   = 0x01,
   WPAD_LED_TWO   = 0x02,
   WPAD_LED_THREE = 0x04,
   WPAD_LED_FOUR  = 0x08,
} WPADLed;
WUT_ENUM_BITMASK_TYPE(WPADLed);

//!  WPAD Infrared Format. For more information see <a href="https://wiibrew.org/wiki/Wiimote#Data_Formats">IR Data Formats</a>
typedef enum WPADDpdFormat
{
   WPAD_DPD_FMT_NONE     = 0,   //!< Disable IR
   WPAD_DPD_FMT_BASIC    = 1,
   WPAD_DPD_FMT_EXTENDED = 3,
   WPAD_DPD_FMT_FULL     = 5,
} WPADDpdFormat;

//! WPAD Speaker Command.
typedef enum WPADSpeakerCmd
{
   WPAD_SPEAKER_CMD_OFF    = 0, //!< Deinitializes and turns off speaker
   WPAD_SPEAKER_CMD_ON     = 1, //!< Turns on and initializes speaker to use 4-bit Yamaha ADPCM data format at 3000 Hz
   WPAD_SPEAKER_CMD_MUTE   = 2, //!< Mutes speaker
   WPAD_SPEAKER_CMD_UNMUTE = 3, //!< Unmutes speaker
   WPAD_SPEAKER_CMD_PLAY   = 4, //!< Allows sound to play
   WPAD_SPEAKER_CMD_ON_ALT = 5, //!< Does the same as WPAD_SPEAKER_CMD_ON
} WPADSpeakerCmd;

//! MotionPlus Mode.
typedef enum WPADMplsMode
{
   WPAD_MPLS_MODE_DISABLE      = 0,
   WPAD_MPLS_MODE_MPLS_ONLY    = 4, //!< MotionPlus exclusive mode
   WPAD_MPLS_MODE_MPLS_NUNCHUK = 5, //!< Nunchuk passthrough mode
   WPAD_MPLS_MODE_MPLS_CLASSIC = 7, //!< Classic passthrough mode
} WPADMplsMode;

typedef enum WPADMplsStatus
{
   WPAD_MPLS_STATUS_ATTACHED   = 0x01, //!< When something is attached to the MotionPlus
   WPAD_MPLS_STATUS_ROLL_CONV  = 0x02, //!< When the roll is in low-velocity mode.
   WPAD_MPLS_STATUS_PITCH_CONV = 0x04, //!< When the pitch is in low-velocity mode.
   WPAD_MPLS_STATUS_YAW_CONV   = 0x08, //!< When the yaw is in low-velocity mode.
   WPAD_MPLS_STATUS_EXT_VALID  = 0x40, //!< When extension data is valid.
   WPAD_MPLS_STATUS_VALID      = 0x80, //!< When MotionPlus data is valid.
} WPADMplsStatus;


//! WPAD Peripheral Memory Space Prefixes
typedef enum WPADPeripheralSpace
{
   WPAD_PERIPHERAL_SPACE_SPEAKER    = 0xA2,
   WPAD_PERIPHERAL_SPACE_EXTENSION  = 0xA4, //!< Any extension other than Motion Plus
   WPAD_PERIPHERAL_SPACE_MOTIONPLUS = 0xA6,
   WPAD_PERIPHERAL_SPACE_DPD        = 0xB0, //!< Infrared
} WPADPeripheralSpace;

//! Balance Board commands.
typedef enum WPADBalanceBoardCmd
{
   WPAD_BALANCE_BOARD_CMD_UPDATE_TEMP = 0x00, //!< Update temperature data.
   WPAD_BALANCE_BOARD_CMD_OFF         = 0x55, //!< Turn Balance Board off.
   WPAD_BALANCE_BOARD_CMD_ON          = 0xAA, //!< Turn Balance Board on.
} WPADBalanceBoardCmd;


//! 2D vector.
struct WPADVec2D
{
   int16_t x;                   //!< x.
   int16_t y;                   //!< y.
};
WUT_CHECK_OFFSET(WPADVec2D, 0x00, x);
WUT_CHECK_OFFSET(WPADVec2D, 0x02, y);
WUT_CHECK_SIZE(WPADVec2D, 0x04);

//! 3D vector.
struct WPADVec3D
{
   int16_t x;                   //!< x
   int16_t y;                   //!< y
   int16_t z;                   //!< z
};
WUT_CHECK_OFFSET(WPADVec3D, 0x00, x);
WUT_CHECK_OFFSET(WPADVec3D, 0x02, y);
WUT_CHECK_OFFSET(WPADVec3D, 0x04, z);
WUT_CHECK_SIZE(WPADVec3D, 0x06);

//! A single IR dot tracked by the camera.
struct WPADIRDot
{
   WPADVec2D pos;               //!< Position (in a 1024x768 grid).
   uint16_t  size;              //!< Apparent size (0-15).
   uint8_t   id;                //!< Identifier.
};
WUT_CHECK_OFFSET(WPADIRDot, 0x0, pos);
WUT_CHECK_OFFSET(WPADIRDot, 0x4, size);
WUT_CHECK_OFFSET(WPADIRDot, 0x6, id);
WUT_CHECK_SIZE(WPADIRDot, 0x8);

/**
 * Core wiimote status.
 *
 * Valid for formats:
 * - `WPAD_FMT_CORE`
 * - `WPAD_FMT_CORE_ACC`
 * - `WPAD_FMT_CORE_ACC_DPD`
 */
struct WPADStatus
{
   uint16_t  buttons;           //!< Bitset from `WPADButton`.
   WPADVec3D acc;               //!< Accelerometer data.
   WPADIRDot ir[WPAD_MAX_IR_DOTS]; //!< IR dots tracked.
   uint8_t   extensionType;     //!< One of `WPADExtensionType`.
   int8_t    error;             //!< Error of the last `WPADRead()`.
};
WUT_CHECK_OFFSET(WPADStatus, 0x00, buttons);
WUT_CHECK_OFFSET(WPADStatus, 0x02, acc);
WUT_CHECK_OFFSET(WPADStatus, 0x08, ir);
WUT_CHECK_OFFSET(WPADStatus, 0x28, extensionType);
WUT_CHECK_OFFSET(WPADStatus, 0x29, error);
WUT_CHECK_SIZE(WPADStatus, 0x2a);

/**
 * Wiimote + Nunchuk status.
 *
 * Note that nunchuk buttons (Z and C) are reported in core.buttons.
 *
 * Valid for formats:
 * - `WPAD_FMT_NUNCHUK`
 * - `WPAD_FMT_NUNCHUK_ACC`
 * - `WPAD_FMT_NUNCHUK_ACC_DPD`
 */
struct WPADStatusNunchuk
{
   WPADStatus core;             //!< Wiimote core state + nunchuk buttons.
   WPADVec3D  acc;              //!< Accelerometer data.
   struct
   {
      int8_t x;                 //!< [-128, 127]
      int8_t y;                 //!< [-128, 127]
   } stick;
};
WUT_CHECK_OFFSET(WPADStatusNunchuk, 0x00, core);
WUT_CHECK_OFFSET(WPADStatusNunchuk, 0x2a, acc);
WUT_CHECK_OFFSET(WPADStatusNunchuk, 0x30, stick.x);
WUT_CHECK_OFFSET(WPADStatusNunchuk, 0x31, stick.y);
WUT_CHECK_SIZE(WPADStatusNunchuk, 0x32);

/**
 * Classic Controller and Classic Controller Pro status.
 *
 * Valid for formats:
 * - `WPAD_FMT_CLASSIC`
 * - `WPAD_FMT_CLASSIC_ACC`
 * - `WPAD_FMT_CORE_ACC_DPD_FULL`
 * - `WPAD_FMT_GUITAR`
 * - `WPAD_FMT_DRUM`
 * - `WPAD_FMT_TAIKO`
 */
struct WPADStatusClassic
{
   WPADStatus core;
   uint16_t   buttons;          //!< Bitset from `WPADClassicButton`.
   WPADVec2D  leftStick;        //!< Left stick: [-512, 511] x [-512, 511]
   WPADVec2D  rightStick;       //!< Right stick: [-512, 511] x [-512, 511]
   uint8_t    leftTrigger;
   uint8_t    rightTrigger;
};
WUT_CHECK_OFFSET(WPADStatusClassic, 0x00, core);
WUT_CHECK_OFFSET(WPADStatusClassic, 0x2a, buttons);
WUT_CHECK_OFFSET(WPADStatusClassic, 0x2c, leftStick);
WUT_CHECK_OFFSET(WPADStatusClassic, 0x30, rightStick);
WUT_CHECK_OFFSET(WPADStatusClassic, 0x34, leftTrigger);
WUT_CHECK_OFFSET(WPADStatusClassic, 0x35, rightTrigger);
WUT_CHECK_SIZE(WPADStatusClassic, 0x36);

/**
 * Pro Controller status.
 *
 * Valid for formats:
 * - `WPAD_FMT_PRO_CONTROLLER`
 */
struct WPADStatusProController
{
   WPADStatus core;
   WUT_PADDING_BYTES(2);
   uint32_t   buttons;          //!< Bitset from `WPADProButton`.
   WPADVec2D  leftStick;        //!< Left stick: [-2048, 2047] x [-2048 x 2047]
   WPADVec2D  rightStick;       //!< Right stick: [-2048, 2047] x [-2048 x 2047]
   BOOL       charging;
   BOOL       wired;
};
WUT_CHECK_OFFSET(WPADStatusProController, 0x00, core);
WUT_CHECK_OFFSET(WPADStatusProController, 0x2C, buttons);
WUT_CHECK_OFFSET(WPADStatusProController, 0x30, leftStick);
WUT_CHECK_OFFSET(WPADStatusProController, 0x34, rightStick);
WUT_CHECK_OFFSET(WPADStatusProController, 0x38, charging);
WUT_CHECK_OFFSET(WPADStatusProController, 0x3c, wired);
WUT_CHECK_SIZE(WPADStatusProController, 0x40);

/**
 * MotionPlus status.
 *
 * Use `core.extensionType` to determine if `nunchuk` or `classic` are valid.
 *
 * Valid for formats:
 * - `WPAD_FMT_MPLUS`
 */
struct WPADStatusMotionPlus
{
   WPADStatus core;
   union
   {
      struct
      {
         WPADVec3D acc;         //!< Accelerometer data.
         struct
         {
            int8_t x;           //!< x: [-128, 127]
            int8_t y;           //!< y: [-128, 127]
         } stick;
      } nunchuk;
      struct
      {
         uint16_t  buttons;     //!< Bitset from `WPADClassicButton`.
         WPADVec2D leftStick;   //!< Left stick: [-512, 511] x [-512, 511]
         WPADVec2D rightStick;  //!< Right stick: [-512, 511] x [-512, 511]
         uint8_t   leftTrigger;
         uint8_t   rightTrigger;
      } classic;
   };
   uint8_t         status;      //!< Bitset from WPADMplsStatus
   WUT_PADDING_BYTES(1);
   WPADVec3D       angle;
};
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x00, core);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x2a, nunchuk.acc);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x30, nunchuk.stick);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x2a, classic.buttons);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x2c, classic.leftStick);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x30, classic.rightStick);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x34, classic.leftTrigger);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x35, classic.rightTrigger);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x36, status);
WUT_CHECK_OFFSET(WPADStatusMotionPlus, 0x38, angle);
WUT_CHECK_SIZE(WPADStatusMotionPlus, 0x3e);

/**
 * Balance Board status.
 *
 * Valid for formats:
 * - `WPAD_FMT_BALANCE_BOARD`
 */
struct WPADStatusBalanceBoard
{
   WPADStatus core;
   uint16_t   pressure[WPAD_MAX_PRESSURE_SENSORS];
   int8_t     temperature;
   uint8_t    battery;
};
WUT_CHECK_OFFSET(WPADStatusBalanceBoard, 0x00, core);
WUT_CHECK_OFFSET(WPADStatusBalanceBoard, 0x2a, pressure);
WUT_CHECK_OFFSET(WPADStatusBalanceBoard, 0x32, temperature);
WUT_CHECK_OFFSET(WPADStatusBalanceBoard, 0x33, battery);
WUT_CHECK_SIZE(WPADStatusBalanceBoard, 0x34);

/**
 * Bullet Train controller status.
 *
 * Valid for formats:
 * - `WPAD_FMT_TRAIN`
 */
struct WPADStatusTrain
{
   WPADStatus core;
   uint16_t   buttons;          //!< Bitset from `WPADClassicButton`.
   uint8_t    brake;            //!< Brake (left) lever.
   uint8_t    throttle;         //!< Throttle (right) lever.
};
WUT_CHECK_OFFSET(WPADStatusTrain, 0x00, core);
WUT_CHECK_OFFSET(WPADStatusTrain, 0x2a, buttons);
WUT_CHECK_OFFSET(WPADStatusTrain, 0x2c, brake);
WUT_CHECK_OFFSET(WPADStatusTrain, 0x2d, throttle);
WUT_CHECK_SIZE(WPADStatusTrain, 0x2e);

//! Controller status info
struct WPADInfo
{
   uint32_t irEnabled;
   uint32_t speakerEnabled;
   uint32_t extensionAttached;
   uint32_t batteryLow;
   uint32_t batteryNearEmpty;
   uint8_t  batteryLevel;
   uint8_t  led;
   uint8_t  protocol;
   uint8_t  firmware;
};
WUT_CHECK_OFFSET(WPADInfo, 0x00, irEnabled);
WUT_CHECK_OFFSET(WPADInfo, 0x04, speakerEnabled);
WUT_CHECK_OFFSET(WPADInfo, 0x08, extensionAttached);
WUT_CHECK_OFFSET(WPADInfo, 0x0c, batteryLow);
WUT_CHECK_OFFSET(WPADInfo, 0x10, batteryNearEmpty);
WUT_CHECK_OFFSET(WPADInfo, 0x14, batteryLevel);
WUT_CHECK_OFFSET(WPADInfo, 0x15, led);
WUT_CHECK_OFFSET(WPADInfo, 0x16, protocol);
WUT_CHECK_OFFSET(WPADInfo, 0x17, firmware);
WUT_CHECK_SIZE(WPADInfo, 0x18);


struct WPADiQueueElement
{
   uint8_t data[0x30];
};
WUT_CHECK_OFFSET(WPADiQueueElement, 0x00, data);
WUT_CHECK_SIZE(WPADiQueueElement, 0x30);

struct WPADiQueue
{
   uint8_t frontIndex;
   uint8_t backIndex;
   WUT_PADDING_BYTES(2);
   WPADiQueueElement* elements;
   uint32_t capacity;
};
WUT_CHECK_OFFSET(WPADiQueue, 0x00, frontIndex);
WUT_CHECK_OFFSET(WPADiQueue, 0x01, backIndex);
WUT_CHECK_OFFSET(WPADiQueue, 0x04, elements);
WUT_CHECK_OFFSET(WPADiQueue, 0x08, capacity);
WUT_CHECK_SIZE(WPADiQueue, 0xc);

//! Bluetooth device address
struct WPADAddress
{
   uint8_t btDeviceAddress[6];
};
WUT_CHECK_OFFSET(WPADAddress, 0x00, btDeviceAddress);
WUT_CHECK_SIZE(WPADAddress, 0x6);

typedef void (*WPADCallback)(WPADChan channel, WPADError status);
typedef WPADCallback WPADControlLedCallback;
typedef WPADCallback WPADControlDpdCallback;
typedef WPADCallback WPADControlSpeakerCallback;
typedef WPADCallback WPADIsMplsAttachedCallback;
typedef WPADCallback WPADGetInfoCallback;
typedef WPADCallback WPADReadMemoryCallback;
typedef WPADCallback WPADWriteMemoryCallback;
typedef WPADCallback WPADConnectCallback;
typedef WPADCallback WPADiSendCallback;
typedef WPADCallback WPADiWriteGameDataCallback;

typedef void (*WPADSamplingCallback)(WPADChan channel);

/**
 * Callback called when the active extension changes
 */
typedef void (*WPADExtensionCallback)(WPADChan channel, WPADExtensionType ext);


/**
 * Initialises the WPAD library for use.
 */
void
WPADInit(void);

/**
 * Cleans up and frees the WPAD library.
 */
void
WPADShutdown(void);

/**
 * Immediately disconnects the associated controller
 */
void
WPADDisconnect(WPADChan channel);

/**
 * Identifies the extension connected to the associated controller
 * \return `WPAD_ERROR_NO_CONTROLLER` if controller is not connected,
 * \return `WPAD_ERROR_NOT_READY` if busy
 */
WPADError
WPADProbe(WPADChan channel,
          WPADExtensionType *outExtensionType);

/**
 * Sets the data format of the controller,
 * can be used to reduce or increase the amount of data received
 * \param channel
 * \param format data format
 * \return `WPAD_ERROR_NONE` on success
 * \return `WPAD_ERROR_NOT_READY` if busy or data
 * \return `WPAD_ERROR_INVALID` if format is for a disabled device type
 */
WPADError
WPADSetDataFormat(WPADChan channel,
                  WPADDataFormat format);

/**
 * Gets the data format in use by the controller
 * \return the current data format
 */
WPADDataFormat
WPADGetDataFormat(WPADChan channel);

/**
 * Reads data for a given wiimote.
 *
 * \param channel
 * \param status Where to store the data; must have be the correct type for the data format.
 * \sa
 * - `WPADSetDataFormat()`
 * - `WPADGetDataFormat()`
 */
void
WPADRead(WPADChan channel,
         WPADStatus *status);

/**
* Controls the associated WPADChan's LEDs
*/
WPADError
WPADControlLed(WPADChan channel,
               WPADLed led,
               WPADCallback callback);

/**
 * Controls the WPADChan's IR sensor
*/
WPADError
WPADControlDpd(WPADChan channel,
               WPADDpdFormat mode,
               WPADCallback callback);

/**
 * Returns the associated controller's IR mode
 */
WPADDpdFormat
WPADGetDpdFormat(WPADChan channel);

/**
 * Controls the associated WPADChan's rumble motor.
 */
void
WPADControlMotor(WPADChan channel,
                 BOOL motorEnabled);

/**
 * Sets the wiimote speaker mode
 */
int32_t
WPADControlSpeaker(WPADChan channel,
                   WPADSpeakerCmd mode,
                   WPADCallback callback);

/**
 * Returns whether the wiimote's speaker is enabled
 */
BOOL
WPADIsSpeakerEnabled(WPADChan channel);

/**
 * Returns whether it is possible to send data to the wiimote's speaker at this moment
 * May return false if device type is unknown, or the device is too busy
 */
BOOL
WPADCanSendStreamData(WPADChan channel);

/**
 * Sends data to be played by wiimote speaker
 * make sure the data is in the format the speaker was initialized for,
 * (4-bit Yamaha ADPCM by default)
 * \param data audio encoded in initialized format
 * \param size number of bytes to send
 * \return `WPAD_ERROR_NOT_READY`, if not possible to send data at this moment
 * \return `WPAD_ERROR_NO_CONTROLLER`, if channel is invalid, data is null or size is more than 20
 */
WPADError
WPADSendStreamData(WPADChan channel,
                   void *data,
                   uint32_t size);

/**
 * Returns the global Wii Remote speaker volume
 */
uint8_t
WPADGetSpeakerVolume(void);

/**
 * Sets the global Wii Remote speaker volume
 * only applies to later initialized Wii Remote speakers
 */
void
WPADSetSpeakerVolume(uint8_t volume);

/**
 * Gets whether MotionPlus is enabled for the WPAD
 * \param enabled is set if MotionPlus is enabled
 */
int32_t
WPADIsMplsAttached(WPADChan channel,
                   BOOL *enabled,
                   WPADCallback callback);

/**
 * Returns whether the WPADChan has MotionPlus integrated
 * \return -1 if controller is not connected, 1 if MotionPlus integrated, 0 if not
 */
int32_t
WPADIsMplsIntegrated(WPADChan channel);

/**
 * Retrieves status info from the controller
 */
WPADError
WPADGetInfo(WPADChan channel,
            WPADInfo *outInfo);

/**
 * Retrieves status info from the controller asynchronously
 */
WPADError
WPADGetInfoAsync(WPADChan channel,
                 WPADInfo *outInfo,
                 WPADCallback callback);

/**
 * Reads from the device's memory
 * \param destination where the received data will be stored
 * \param size number of bytes to read
 * \param address
 * device memory address, see
 * <a href="https://wiibrew.org/wiki/Wiimote#EEPROM_Memory">EEPROM Memory</a> and
 * <a href="https://wiibrew.org/wiki/Wiimote#Control_Registers">Control Registers</a>
 * \param completionCallback function to be called upon completion
 * \sa
 * - WPADWriteMemoryAsync()
 * - WPADReadExtReg()
 */
WPADError
WPADReadMemoryAsync(WPADChan channel,
                    void *destination,
                    uint16_t size,
                    uint32_t address,
                    WPADCallback callback);

/**
 * Writes to the device's memory
 * \param source data to be written to the controller
 * \param size number of bytes to write
 * \param address
 * device memory address, see
 * <a href="https://wiibrew.org/wiki/Wiimote#EEPROM_Memory">EEPROM Memory</a> and
 * <a href="https://wiibrew.org/wiki/Wiimote#Control_Registers">Control Registers</a>
 * \param callback function to be called upon completion
 * \sa
 * - WPADReadMemoryAsync()
 * - WPADWriteExtReg()
 */
WPADError
WPADWriteMemoryAsync(WPADChan channel,
                     void *source,
                     uint32_t size,
                     uint32_t address,
                     WPADCallback callback);

/**
 * Reads from the registers of the Wii Remote's peripherals
 * \param address address within the peripheral's memory space
 * \sa
 * - WPADReadMemoryAsync()
 * - WPADWriteExtReg()
 */
WPADError
WPADReadExtReg(WPADChan channel,
               void *destination,
               uint16_t size,
               WPADPeripheralSpace peripheral,
               uint32_t address,
               WPADCallback callback);

/**
 * Writes to the registers of the Wii Remote's peripherals
 * \param address address within the peripheral's memory space
 * \sa
 * - WPADWriteMemoryAsync()
 * - WPADReadExtReg()
 *
 * Usage:
 * \code
 * // Setting speaker volume on specific controller
 * uint8_t volume;
 * volume = 0x40;
 * WPADWriteExtReg(WPAD_CHAN_0, &volume, 1, WPAD_PERIPHERAL_SPACE_SPEAKER, 0x05, nullptr);
 * \endcode
 */
WPADError
WPADWriteExtReg(WPADChan channel,
                const void *source,
                uint32_t size,
                WPADPeripheralSpace peripheral,
                uint32_t address,
                WPADCallback callback);

/**
 * Read Balance Board calibration
 * \param address may only be in the ranges [0x24,0x40) and [0x50, 0x70)
 * \sa
 * - WPADReadExtReg()
 */
WPADError
WPADGetBLCalibration(WPADChan channel,
                     void *destination,
                     uint32_t address,
                     uint32_t size,
                     WPADCallback callback);
/**
 * Sets power save mode, this makes the controller only report input data
 * when it changes
 */
void
WPADSetPowerSaveMode(WPADChan channel,
                     BOOL powerSave);

/**
 * \return FALSE, if power save mode is off
 */
BOOL
WPADGetPowerSaveMode(WPADChan channel);

/**
 * Retrieves the bluetooth address of the controller
 */
void
WPADGetAddress(WPADChan channel,
               WPADAddress* outAddress);

/**
 * Enables/disables motors globally
 */
void
WPADEnableMotor(BOOL enable);

/**
 * Returns whether motors are disabled globally
 */
BOOL
WPADIsMotorEnabled(void);

/**
 * Enables/disables Wii U Pro Controller support
 */
void
WPADEnableURCC(BOOL enable);

/**
 * Returns whether Wii U Pro Controllers are supported
 */
BOOL
WPADIsEnabledURC(void);

/**
 * Enables/disables Wii Balance Board support
 */
void
WPADEnableWBC(BOOL enable);

/**
 * Returns whether Wii Balance Boards are supported
 */
BOOL
WPADIsEnableWBC(void);

/**
 * Enables/disables Wii Remote support
 */
void
WPADEnableWiiRemote(BOOL enable);

void
WPADSetAutoSleepTime(uint8_t time);

/**
 * Starts searching for a WPAD controller in pairing mode and syncs with it
 * \return TRUE if sync started
 */
BOOL
WPADStartSyncDevice(void);

/**
 * Starts attempts to sync with a WPAD with the specified properties,
 * if unable to do so, starts a normal sync as with WPADStartSyncDevice
 * \param deviceAddress Bluetooth address of the device to connect to.
 * \param deviceName Bluetooth name of the device to connect to (up to 24 characters)
 * \return TRUE if sync started
 *
 * Usage:
 * \code
 * WPADAddress addr;
 * memset(&addr, 0x10, 6);
 * // Initially searches for device with address 10:10:10:10:10:10 and name "Nintendo"
 * WPADStartSyncDeviceEx(&addr, "Nintendo");
 * \endcode
 */
BOOL
WPADStartSyncDeviceEx(WPADAddress* deviceAddress,
                      const char* deviceName);

/**
 * Set function to be run upon controller connect/disconnect
 * \param callback Callback to be invoked; status will be:
 *   - `WPAD_ERROR_NONE` when controller connects.
 *   - `WPAD_ERROR_NO_CONTROLLER` when controller disconnects.
 * \return the previously used callback
 */
WPADConnectCallback
WPADSetConnectCallback(WPADChan channel,
                       WPADCallback callback);
/**
 * Set the function to be run upon extension connect and motion plus activation
 * \return the previously used callback
 */
WPADExtensionCallback
WPADSetExtensionCallback(WPADChan channel,
                         WPADExtensionCallback callback);

WPADSamplingCallback
WPADSetSamplingCallback(WPADChan channel,
                        WPADSamplingCallback callback);

void
WPADiShutdown(void);

/**
 * Clears all elements from queue
 */
void
WPADiClearQueue(WPADiQueue *queue);

/**
 * Checks if there is enough space in the queue
 */
bool
WPADiIsAvailableCmdQueue(WPADiQueue *queue,
                         uint32_t count);

/**
 * Parses incoming HID report data for a controller
 * \return -1 if first byte is outside the valid input report range (0x20 to 0x3f)
 */
int32_t
WPADiHIDParser(WPADChan channel,
               uint8_t *hidData);


/**
 * Queues HID Report for Rumble Update
 *
 * Rumble must be set before this
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendSetVibrator(WPADiQueue *cmdQueue);

/**
 * Queues HID Report for setting LEDs
 * used internally by \link WPADControlLed \endlink
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendSetPort(WPADiQueue *cmdQueue,
                 WPADLed led,
                 WPADCallback callback);

/**
 * Queues HID Report for setting data reporting mode
 * used internally by \link WPADSetPowerSaveMode \endlink
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendSetReportType(WPADiQueue *cmdQueue,
                       WPADDataFormat dataFormat,
                       BOOL powerSave,
                       WPADCallback callback);

/**
 * Queues HID report for a controller status request
 * used internally by \link WPADGetInfoAsync \endlink and several other functions
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendGetContStat(WPADiQueue *cmdQueue,
                     WPADInfo *outInfo,
                     WPADCallback callback);

/**
 * Queues HID Report for enabling the IR Camera clock
 * used internally by \link WPADControlDpd \endlink
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendEnableDPD(WPADiQueue *cmdQueue,
                   BOOL enable,
                   WPADCallback callback);

/**
 * Queues HID Report for enabling IR Camera
 * used internally by \link WPADControlDpd \endlink
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendEnableDPDCSB(WPADiQueue *cmdQueue,
                      BOOL enable,
                      WPADCallback callback);

/**
 * Queues HID Report for enabling speakers
 * used internally by \link WPADControlSpeaker \link
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendEnableSpeaker(WPADiQueue *cmdQueue,
                       BOOL enable,
                       WPADCallback callback);

/**
 * Queues HID Report for muting speakers
 * used internally by \link WPADControlSpeaker \link
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendMuteSpeaker(WPADiQueue *cmdQueue,
                     BOOL mute,
                     WPADCallback callback);

/**
 * Queues HID Report for sending speaker stream data
 * used internally by \link WPADSendStreamData \endlink
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendStreamData(WPADiQueue *cmdQueue,
                    void *source,
                    uint32_t size);

/**
 * Queues HID Report for a single-byte memory write
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendWriteDataCmd(WPADiQueue *cmdQueue,
                      uint8_t byte,
                      uint32_t address,
                      WPADCallback callback);

/**
 * Queues HID Report for a multi-byte memory write
 * used internally by \link WPADWriteMemory \endlink
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendWriteData(WPADiQueue *cmdQueue,
                   void *source,
                   uint32_t size,
                   uint32_t address,
                   WPADCallback callback);

/**
 * Queues HID Report for a memory read
 * used internally by \link WPADReadMemory \endlink
 * \return TRUE if successfully added to queue
 */
BOOL
WPADiSendReadData(WPADiQueue *cmdQueue,
                  void *destination,
                  uint16_t size,
                  uint32_t address,
                  WPADCallback callback);

/**
 * Game code (identifier), which may be saved to the EEPROM of connected controllers
 * \return pointer to the game code
 */
uint32_t*
WPADiGetGameCode(void);

/**
 * Game type, which may be saved to the EEPROM of connected controllers
 * \return 0x80
 */
uint8_t
WPADiGetGameType(void);

/**
 * Sets game title for all connected controllers
 * \param title up to 17 UTF-16 characters including null terminator
 * title will be copied onto the controller EEPROM
 * \sa
 * - WPADGetGameTitleUtf16
 * - WPADiWriteGameData
 */
void
WPADSetGameTitleUtf16(uint16_t *title);

/**
 * Gets game title stored on specified controller
 * \param outTitle pointer to where the title will be output
 * \return `WPAD_ERROR_INVALID`, if game data previously failed to write
 * \sa
 * - WPADSetGameTitleUtf16
 * - WPADiReadGameData
 */
WPADError
WPADGetGameTitleUtf16(WPADChan channel,
                      uint16_t **outTitle);

/**
 * Get the time that game data was written
 * \return `WPAD_ERROR_INVALID`, if game data previously failed to write
 */
WPADError
WPADGetGameDataTimestamp(WPADChan channel,
                         OSTime *timestamp);

/**
 * Write custom game data to the controller's EEPROM
 *
 * \param offset start address within custom data region
 * \param callback Invoked when write completes; status will be:
 *   - `WPAD_ERROR_NONE` on success.
 *   - `WPAD_ERROR_TRANSFER` on failure.
 *
 * also stores the current game type and game code and commits the game title set by \link WPADSetGameTitleUtf16 \endlink
 * \return `WPAD_ERROR_NONE`, if the write request was sent
 * \return `WPAD_ERROR_NOT_READY`, if the controller is busy, or game data is in the process of being read or written
 * \sa
 * - WPADiReadGameData
 * - WPADiGetGameType
 * - WPADiGetGameCode
 * - WPADGetGameDataTimestamp
 */
WPADError
WPADiWriteGameData(WPADChan channel,
                   void *source,
                   uint16_t size,
                   uint32_t offset,
                   WPADCallback callback);

/**
 * Read custom game data from the controller's EEPROM
 * \param offset start address within custom data region
 * \return `WPAD_ERROR_NONE`, if the read request was sent
 * \return `WPAD_ERROR_NOT_READY`, if the controller's game data is in the process of being read or written
 * \return `WPAD_ERROR_PERMISSION`, if the WPAD's GameCode does not match the global Game Code
 * \return `WPAD_ERROR_BROKEN`, if game data previously failed to write
 * \sa
 * - WPADiWriteGameData
 * - WPADiGetGameCode
 */
WPADError
WPADiReadGameData(WPADChan channel,
                  void *destination,
                  uint16_t size,
                  uint32_t offset,
                  WPADCallback callback);

/**
 * Get MotionPlus mode
 *
 * identical to \link KPADGetMplsStatus \endlink
 */
WPADMplsMode
WPADiGetMplsStatus(void);

/**
 * Sends a command to the balance board.
 */
WPADError
WPADControlBLC(WPADChan channel,
               WPADBalanceBoardCmd command,
               WPADCallback callback);

// TODO:
//WBCGetABSWeight

/**
 * Converts raw battery value to a level.
 *
 * \param battery The `battery` field from `WPADStatusBalanceBoard`.
 * \return A level from 0 to 4.
 */
int32_t
WBCGetBatteryLevel(uint8_t battery);

/**
 * Checks if the Balance Board calibration is completed.
 *
 * \return TRUE if calibration is completed, FALSE if in progress, or an error occurred.
 * \sa `WBCSetupCalibration()`
 */
BOOL
WBCGetCalibrationStatus(void);

// TODO:
//WBCGetGCWeight
//WBCGetGravCoff
//WBCGetProductArea
//WBCGetTCWeight

/**
 * Applies temperature and gravity correction to weight value.
 *
 * \param inputWeight The input weight value.
 * \param correctedWeidht Where to store the corrected weight.
 * \param status The status data used in `WPADRead()` and `WBCRead()`.
 * \return Zero on success, or an error code.
 */
int32_t
WBCGetTGCWeight(double inputWeight,
                double *correctedWeight,
                WPADStatusBalanceBoard *status);

//WBCGetZEROPoint

/**
 * Converts the raw pressure data into weight values.
 *
 * \param status The status argument filled in by `WPADRead()`.
 * \param weight Pointer to an array of 4 elements to store the weights.
 * \param count How many elements in `weight` (should be 4).
 * \return Zero on success, or negative error code.
 * \sa `WBCSetupCalibration()`
 */
int32_t
WBCRead(WPADStatusBalanceBoard *status,
        double weight[],
        uint32_t count);

/**
 * Resets the zero weight value.
 *
 * \param zeros Array raw pressure values that correspond to zero weight, one for each sensor.
 * \param count The size of the `zeros` array, should be 4.
 * \sa
 * - `WBCSetupCalibration()`
 * - `WPADControlBLC()`
 */
int32_t
WBCSetZEROPoint(double zeros[],
                uint32_t count);

/**
 * Starts the calibration of Balance Board.
 *
 * Note: this function is asynchronous.
 * \return TRUE if calibration started successfully, FALSE if it failed to start.
 * \sa `WBCGetCalibrationStatus()`
 */
BOOL
WBCSetupCalibration(void);

#ifdef __cplusplus
}
#endif

/** @} */
