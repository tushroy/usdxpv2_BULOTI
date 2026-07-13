//  QCX-SSB.ino - https://github.com/threeme3/QCX-SSB
//
//  Copyright 2019, 2020, 2021   Guido PE1NNZ <pe1nnz@amsat.org>
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  Copyright all additions 2022-2023 Rob Colclough GW8RDI, use of the additions and changes is permitted for all private, non-commecial use at user´s risk.  No responsibility is accepted for any losses that may occur through the use of this modified code.

// THIS CODE SUPERCEEDS VERSIONS 1.02X AND 1.03. 1.03 HAS UPDATES FOR OLED CHIPSETS BUT DOES NOT HAVE FUNCTIONAL OR DSP CHANGES COMPARED TO 1.02w.

// GW8RDI IMPORTANT NOTES: ***   DO NOT RUSH - READ THE NOTES BELOW SEVERAL TIMES!

// *** THIS OPEN SOFTWARE IS FULLY SUPPORTED* FREE OF COST BY GW8RDI and others ***

/* *********      HEY, DON'T BE LAZY, READ THE NOTES!!       *********

SEE BELOW THE TEXT ENTRIES "!CHANGE" and replace those 2 lines with your callsign, and the one with the lenght!  Use search or find with the text !CHANGE

Don't forget to change the length to match your callsign if needed.

*/

// *** ISP DATA CORRUPTION WARNING1!!   ALWAYS REMOVE C24 (C27?) (or as marked) it's a 10nF on the ISP HEADER'S MOSI line (PA ctrl out) (PB3) of the ISP header,
//  and disconnect the internal mic by plugging in a disconnected Jack plug.  FAILING TO DO THIS CAN CORRUPT YOUR MCU CHIP!!

// *** CHECK WITH ME FIRST PLEASE ***

// PLEASE DO NOT PUBLISH WHAT YOU THINK ARE FAULTS WITH THIS RELEASE *** BEFORE CHECKING WITH ME, GW8RDI ***.
// 99% OF REPORTED PROBLEMS ARE JUST CONFIGURATION RELATED, NOT BUGS OR LIMITATIONS.
// SO IF YOU USE THIS SOFTWARE, YOU AGREE NOT TO PUBLISH UNTIL YOU´VE CONTACTED ME, GW8RDI
// THIS AVOIDS CONFUSIONS.

// Supports up to 9 bands (using 8 filters, 15 and 17M share). To increase change N_BANDS, and add frequencies to array "band[N_BANDS] ="
// SSB TX quality tests, see below MORE_MIC_GAIN, QUAD

/*  WELCOME TO THE OPEN SOURCE USDX PROJECT 2022 AND ONWARDS!

	This version is for all uSDX/uSDR transceivers, release numbers are 4.XXx.

	Compiled and programmed using Arudino IDE 2.0.4, available from www.arduinio.cc

	To support the add-on module for uSDX with powerful DSP processor, version numbers will be 3.XXx.

	*** READ ALL THE NOTES here and ask questions on the FB "USDX USDR" group before programming, it may save a lot of heartache!
	Our intention is to make the uSDX a great option, with all the facilities and interfaces you can imagine!
	The quadrature mixer (designed by Dan Tayloe N7VE) in the uSDX makes it as good a receiver as a professional rig, when built correctly.
	The TX modulation has been improved and gets good reports, and with the add-on module it can be as good as the best out there.
	Enjoy!
	73 Rob, GW8RDI
*/

//  G8RDI Modifications log:
#define VERSION   "4.00e"    // Fixed format "9.99z" : Additions and changes Copyright 2022-2024 GW8RDI - You can use and distribute if you maintain the copyright message, commercial use is prohibited.

//  2022/03/04 - Added delay to show serial number at start - G8RDI mod
//               Added band change direction based on last freq step directions. See "case BE | DC:" - GW8RDI mod
//               Set PB3/PB5 to output in init to drive LCD backlight
//  2022/03/05 - Release 1.02wA2 2022/03/20 : Added Cat 8.6 ad QUAD enable 8.7 menu items
//  2022/03/06 - With MORE_MIC_GAIN enabled & QUAD disabled, SSB TX voice is sounding much better!
//  2022/04/24 - Release 1.02wA3: Fixed band dir. bug
//  2022/05/08 - Added KEEP_BAND_DATA to maintain last freq and mode set on each band.
//  2022/05/09 - Release 1.02wA4 : Maintains last freq and mode set for each band, up to 9 bands set. Added error code display.
//  2022/05/11 - Changed menu to cycle end-start, start-end
//  2022/07/19 - Added full CW mode update on band change, minor fixes
//  2022/07/19 - Release 1.02wA5 : Maintains last freq and mode set for each band, up to 9 bands set. Added error code display.
//  2022/07/28 - Release 1.02wA6 : Minor changes. Reduced CW message duplication to release more memory.
//  2022/08/16 - Release 1.02wA7 : Directional band-change went in the wrong direction if SWAP_ROTARY was not enabled. This fix solves that issue.
//  2022/08/19 - Release 1.02wA8 : Directional band-change saved last band-mode to Flash, and not mode of band changed to, causing incorrect mode restore if shutdown without band change.  Minor other changes, DEBUG modes changes callsign.
//  2022/08/22 - Release 1.02wA8a : Red Corners Unit: CW messages updated, Band menu fix as was showing 6m in the list. Added WHITE_CORNERS to config list.
//  2022/08/22 - Release 2.00a : Jumped version to avoid confusion with prev. releases. Added SWR, 115200 baud CAT, experimental changes to AM and FM, and minor tidy up
//  2022/10/12 - Release 2.00b : NR stays as last used.
//  2022/11/06 - Release 2.00c : #define KEYER CW keyer for Iambic -> NOTE: Auto CW msg sending aborts if not installed, and 700Hz filter selection gives reduced gain.
//  : Added new post mag IQ filter, added BlackBrick config.
//  2023/02/09 - Release 2.00d : Added CAT freq. error handling, in RIT mode the TX freq. is now displayed, plus FIR noise filter, see #define NR_FIR below. NR 0-2 is 1.02x method, 3-8 is now FIR DSP filter method to be used with Full b/w and Att2 = 2
//  2023/02/09 - Release 2.00e : Ammended code to support +/- 99 KHz RIT receiver offset, and added CAT command to set the RIT offset.
//  2023/03/14 - Release 2.00f : Ammended code with new "TT" CAT command to set a TX offset frequency; see CAT_XO_CMD. Minor improvements for setting mode display, etc.
//  2023/03/14 - Update to 2.00f : Ammended code as missing #ifdef KEEP_BAND_DATA on line 5922
//  2023/04/01 - Release 4.00a : Changed to version 4.00 as DL2MAN jumped his version up to 2.00!  CAT now enables directly from the menu without needing a reboot to activate!  CAT MDx; now refreshes LCD fully after CW mode.
//                               Re-coded KEEP_BAND_DATA switch statements which freed 328 bytes!!
//  2023/04/11 - Release 4.00b : Fixed CAT mode change to allow it to select AM and FM, AM important to correctly track IQ flip. Also commented-out redundant BE | DC for mode change, as now used for directional band change.
//                               RIT mode now allows mode to be changed.
//  2023/04/17 - Release 4.00c : Ammended configuration for TRUSDX clone so that latched-relay band switching and SWR selection is included.
//  2023/04/18 - Release 4.00d : Minor updates to handle both 5 and 8 band versions of (tr)usdx clone

//  2024/12/07 - Release 4.00e : v_FWD = v_FWD / 6;	// Mentioned by Ovidiu Băluță that Blackbrick needs a high level
//														 : Code to reduce memory usage: fastfastpinMode() and fastfastdigitalWrite()

//  : Added new post mag IQ filter, added BlackBrick config.
//  : todo see "// xyzzy Test with i_d"

// todo - notes: pinmode could be replaced with more compact code. See source at https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/wiring_digital.c

// NOTE update #define VERSION "????" above!

// *** Use of this modified software is at the users risk ***  PLEASE READ THE INSTRUCTIONS AVAILABLE IN THE FB GROUP "uSDX uSDR Radios" or uSDX Group IO online.

// Notes: To have CW tone in the menu, enable #define FILTER_700HZ. You´ll need to find 28 bytes depending on your config, i.e. disabling #define DIAG

// Configuration switches; remove/add a double-slash at line-start to enable/disable a feature; to save space disable e.g. CAT, DIAG, KEYER

// *** BEFORE ALL, READ THIS BLOCK !!!!!!!!

/* THIS CONFIG: / To change, ADD or DELETE the "//" in front of #define lines below
		XXBLACK BRICK UNIT, FRONT PANEL ON LARGER SIDE: 25.0, SWR, SWAP-ROTARY, BACKLIGHT_PIN 0x08
		RED BUTTONS UNIT 27001400 (CHANGE BELOW TO SET YOUR OFFSET OR SET 27000000), NO SWR, SWAP-ROTARY
		XXWHITE BUTTONS UNIT 27000000 (CHANGE BELOW TO SET YOUR OFFSET OR SET 27000000), NO SWR, SWAP-ROTARY - NEEDS CHECKING TODO
		XXRED CORNERS UNIT 25000000 SWR, SWAP-ROTARY
		XXTRUESDX UNIT - PLEASE CONTACT GW8RDI TO CHECK YOUR CONFIGURATION BEFORE PROGAMMING AS WE ARE IN TESTING PHASE.
*/
// NOTE: ONLY ENABLE ONE OF THE MODELS BELOW BY ADDING OR REMOVING THE UNCOMMENT "//"
//________________________________________________________________________________________________________________________
//#define BLACK_BRICK 1   // Backlight control PortD is PD3 0x08, SWR, NO ROTARY SWAP
//#define RED_CORNERS 1 // Backlight control PortD is PD3 0x08, or PD5 0x20 for Red Corners rig. Disable for Red and White buttons and most black brick uSDX.

// !!!!! ALWAYS DISABLE LINE BELOW !!!!!!
///#define MY_RED_CORNERS 1	// Only for my(GW8RDI) Red Corners with reversed rotary part!
// !!!!! ALWAYS DISABLE LINE ABOVE !!!!!!

//#define RED_BUTTONS 1 // Used for Small HF SDR TRANSCEIVER uSDX model, without SWR circuit.  May have SMD inductors.
 
#define WHITE_BUTTONS 1 // Small black unit with white or red buttons on front, without SWR circuit.
#define SWAP_ROTARY 1   // Enabled globally for White Buttons configuration to fix tuning and band change direction
#define REVERSE_BAND_CHANGE 1 // Revere button change enabled

//#define TRUSDX 1 // Small USDX clone in 3D printed case marked "DL2MAN & PE1NNZ".  CHECK WITH GW8RDI BEFORE USING THIS FOR UPDATES AND CONFIG DETAILS!  SWR protection via PA sensing resistor can be added if needed.
// NOTE:  DL2MAN claims (as of 17 April 2023) that his license blocks users from installing other software (Microsoft vs Linux et al), but it is understood that this violates consumer rights laws in the USA, UK and European Union.

// *** NOTE ***: If none of the above are enabled, configuration may match other units, but if tuning direction is reversed, backlight or frequency wrong, adjust as needed.
// IF IN DOUBT PLEASE ASK ME FIRST: GW8RDI
//________________________________________________________________________________________________________________________

////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 03/01/2026 - 9M2GRC
// Define bands selection 
////////////////////////////////////////////////////////////////////////
#ifdef TRUSDX
	#define LOBAND
	//#define CLASSICBAND
	//#define HIBAND
	#define NCHGFILTDC
	//#define EXPSSB
#endif // TRUSDX

#if defined(RED_CORNERS) || defined(BLACK_BRICK)
#define BACKLIGHT_PIN 0x20
#else
#define BACKLIGHT_PIN 0x08
#endif

#ifndef TRUSDX  // With CAT enabled on trusdx we remove band data and mode memory to free space
#define KEEP_BAND_DATA 1        // Maintain last freq and mode set on each band - GW8RDI mod
#endif

#define SHOW_USB_LSB_CW_ONLY 1  // If defined, Menu will only cycle thro these 3 modes

// AM & FM Modulation changes
//#define FM_ARCTAN 1         // Enable FM differentiator TEST - GW8RDI mod
//#define AM_MOD_MAGN_SQRT 1  // Use more accurate SQRT method

// 2025/12/07 GW8RDI: CODE TO REDUCE PROG MEMORY:-

/**
 * @brief Custom pinMode replacement using direct register access
 * @GW8RDI Dec 2025 - 
 * @details Saves flash memory compared to the standard Arduino pinMode()
 * @param pin The standard Arduino pin number (0-19)
 * @param mode INPUT, OUTPUT, or INPUT_PULLUP
 * @note This can be reduced to the following to save more memory:
 		pinMode(11, OUTPUT);-> DDRB	= _BV(PB3);
 */
void fastpinMode(uint8_t pin, uint8_t mode) {
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t* reg, * out;

	if (port == NOT_A_PIN)
		return;

	reg = portModeRegister(port);  // Pointer to DDRx
	out = portOutputRegister(port); // Pointer to PORTx

	if (mode == INPUT) {
		*reg &= ~bit; // Clear DDRx bit (Input)
		*out &= ~bit; // Clear PORTx bit (Disable Pull-up)
	}
	else if (mode == INPUT_PULLUP) {
		*reg &= ~bit; // Clear DDRx bit (Input)
		*out |= bit;  // Set PORTx bit (Enable Pull-up)
	}
	else { // OUTPUT
		*reg |= bit;  // Set DDRx bit (Output)
	}
}

/**
 * @brief Custom digitalWrite replacement using direct register access
 * @GW8RDI Dec 2025
 * @details Saves flash memory compared to the standard Arduino fastdigitalWrite()
 * @param pin The standard Arduino pin number (0-19)
 * @param val HIGH or LOW
  * @note This can be reduced to the following to save more memory:
		digitalWrite(11, HIGH);	-> PORTB	= _BV(PB3);
		digitalWrite(11, LOW);	-> PORTB &= ~_BV(PB3);	Sets Pin PB3 LOW
 */
void fastdigitalWrite(uint8_t pin, uint8_t val) {
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t* out;

	if (port == NOT_A_PIN)
		return;

	out = portOutputRegister(port); // Pointer to PORTx

	if (val == LOW) {
		*out &= ~bit; // Clear PORTx bit (Set Low)
	}
	else { // HIGH
		*out |= bit;  // Set PORTx bit (Set High)
	}
}

//****************************************************************
//#define DEBUG_G8RDI 1   // Enables display of error codes on LCD
//****************************************************************

// Change Callsign from G8RDI to match your own, or set it to "uSDR+  " if you don't want it customised:-

// *** CALLSIGN NO MORE THAN 5 CHARACTERS!!! DON'T REMOVE THE 2 SPACES!!!  IGNORE THIS WARNING IT WILL CRASH THE PROGRAM!!! ***
#ifdef DEBUG_G8RDI
#define MY_CALLSIGN_PADDED "DEBUG  "
#else
// Put your callsigne below and remove the "///" in front to activate.
#define MY_CALLSIGN "S21TIP"               // <----- !CHANGE TO your callsign here or enable line below, replacing G8RDI!  If you don´t want the LCD to show your callsign, enable the line "uSDR+" below.

#define MY_CALLSIGN_PADDED "S21TIP  "      // <----- !CHANGE TO your callsign here BUT keep the 2 spaces at the end!
/// Disable below line if using your own callsign by adding // in front.
//#define MY_CALLSIGN_PADDED "uSDR+  "    // Ensure two spaces at end of heading and that it is under 7 characters (including the 2 spaces), or this program may not work correctly.

//#define MY_PREFIX ""  // No prefix, use this line by removing the //, add below to replica line.
#define MY_PREFIX ""    // Add visiting country prefix here

#define MY_NAME "TUSHAR"   // <---- *** ADD YOUR NAME HERE FOR CW MESSAGES
#endif
#define CALLSIGN_LENGTH 6       // !CHANGE length to match your callsign but remember the LCD isn't very wide!

// *** MEMORY LIMITATION OF ATMEGA328 *** This means you may have to mix and match functions option defines.  CAT requires considerable memory, so use only if needed.

#if defined(RED_CORNERS) || defined(BLACK_BRICK)
#define SWR_METER      1   // Supports SWR meter with bridge on A6/A7 (LQPF ATMEGA328P) by Alain, K1FM, see: https://groups.io/g/ucx/message/6262 and https://groups.io/g/ucx/message/6361
#endif

//***************** TRUSDX FEATURES - OR USDX WITH OLED
#if defined(TRUSDX)

//#define LCD_I2C        1   // LCD with I2C (PCF8574 module          ), connect SDA (PD2), SCL (PD3), NOTE that this display is pretty slow

#define OLED_SSD1306     1   // OLED display (SSD1306 128x32 or 128x64), connect SDA (PD2), SCL (PD3)
//#define OLED_SH1106    1   // OLED display (SH1106 1.3" inch display), connect SDA (PD2), SCL (PD3), NOTE that this display is pretty slow
//#define CONDENSED        1   // Display in 4 line mode (for OLED and LCD2004 modules)

//#define LPF_SWITCHING_DL2MAN_USDX_REV3 1    // Default 5-8 band latching relays IM43
//#define LPF_SWITCHING_DL2MAN_USDX_REV2  1 // 5 band latching relays IM43

#define SWR_METER        1   // Supports SWR meter with bridge on A6/A7 (LQPF ATMEGA328P) by Alain, K1FM, see: https://groups.io/g/ucx/message/6262 and https://groups.io/g/ucx/message/6361

//*****************
#else

#define LPF_SWITCHING_DL2MAN_USDX_REV3 1    // Default 8 band latching relays IM43
//#define LPF_SWITCHING_DL2MAN_USDX_REV3_NOLATCH 1    // NOTE: CHANGE IF THIS VERSION LATCHES
//#define LPF_SWITCHING_DL2MAN_USDX_REV2  1 // 5 band latching relays IM43

#endif // TRUSDX

#if defined(BLACK_BRICK)
#define SWR_METER      1   // Supports SWR meter with bridge on A6/A7 (LQPF ATMEGA328P) by Alain, K1FM, see: https://groups.io/g/ucx/message/6262 and https://groups.io/g/ucx/message/6361
#endif

//#define FAST_AGC         1   // Adds fast AGC option (good for CW) Slow mode not recommended.  Remove for CAT if memory errors.

#define CAT              1   // CAT-interface - OTHER OPTIONS, SUCH AS CW_MESSAGES and KEEP_BAND_DATA MAY TO BE DISABLED TO MAKE SPACE FOR CAT
//#define CAT_EXT        1   // Extended CAT support: remote button and screen control commands over CAT
//#define CAT_STREAMING    1   // Streams audio and IQ, only 8KHz b/w, & needs faster 115200 baud RS232
#define CAT_FAST         1   // Uses faster 115200 baud (can be changed to 57600), else 38400, 8, 1, N.

// If short of memory on compile and not using Spectrum display, disable CAT_XO_CMD:- Like this:-> //#define CAT_XO_CMD
#ifdef CAT

#define CAT_TX_CMD          1  // GW8RDI mod - added - Send TX and RX status CAT cmds as PTT is pressed and released
#define CAT_XO_CMD          1  // GW8RDI mod - added - Set TX offset freq. for Quantum Spectrum module from QuantumSDR.com
// Note: to use CAT_XO_CMD, RIT_ENABLE must also be enabled.
#endif

// Lines below NEEDED FOR CW, removed to make space for CAT
#define KEYER            1   // CW keyer for Iambic - NOTE: Auto CW msg sending aborts if not installed as changes dit timing. Can be removed to save memory for CAT
#define KEY_CLICK        1   // G8RDI mod - may be removed to free memory for CAT - NEEDED FOR CW msg sending else CW TX sounds mushy & CW msg sending stops after one peep! // Reduce key clicks by envelope shaping
//#define FILTER_700HZ   1    // G8RDI mod - Moved here - Enabled shows in Menu

// CW Messages: Note: If CAT is enabled, CW messages may cause a program memory overflow. KEEP_BAND_DATA can be disabled to release memory for CW at cost of losing band frequency memory.
//#define CW_MESSAGE 1          // Transmits pre-defined CW messages on-demand (left-click menu item 4.2)
#define CW_MESSAGE_EXT 1      // Additional CW messages

// NOTE: DO NOT CHANGE THE CW_MESSAGE LINES BELOW AS THEY ARE INCORPORATED OR NOT BASED ON CW_MESSAGE and CW_MESSAGE_EXT above.
// Note: !!!Do not exceed CW_MESSAGE_LENGTH when ammending messages!!!
#ifdef CW_MESSAGE_EXT
#define CW_MESSAGE_LENGTH 48  //48/32
#else
#define CW_MESSAGE_LENGTH 48  //48/32/16
#endif

// CHANGE THE CW MESSAGE TEXT BELOW AS YOU LIKE BUT LESS THAN CW_MESSAGE_LENGTH+1 CHARS!  THE ## MEANS +, USED TO CONCATANTE STRINGS.
// DO NOT COMMENT OUT LINES BELOW, THESE TEXTS ARE NOT INCLUDED WHEN CW_MESSAGES/EXT ARE DISABLED
#define CW_STD_MSG "CQ CQ DE " MY_CALLSIGN " +"  // 16 chars, change in code of size changes.
#define CW_MSG1 "CQ CQ DE " MY_CALLSIGN " +"
#define CW_MSG2 "CQ CQ DE " MY_PREFIX MY_CALLSIGN " +"
#define CW_MSG3 MY_PREFIX MY_CALLSIGN
#define CW_MSG4 "GE TKS 5NN 5NN NAME IS " MY_NAME " HW?"
#define CW_MSG5 "FB RPTR TX 5W ANT EFW 73 CUAGN"
#define CW_MSG6 "73 GL TU EE"
// Examples:
//"CQ"  MY_CALLSIGN " +", "CQ CQ DE " MY_CALLSIGN + MY_CALLSIGN " +", "GE TKS 5NN 5NN NAME IS ROB ROB HW?", "FB RPTR TX 5W 5W ANT ENDFED 73 CUAGN", "73 TU E E", "G8RDI"
//"CQ CQ DE " MY_CALLSIGN " " MY_CALLSIGN " +", "GE TKS 5NN 5NN NAME IS " MY_NAME "" MY_NAME " HW?", "FB RPTR TX 5W 5W ANT ENDFED 73 CUAGN", "73 TU E E", MY_CALLSIGN
//#define CW_MSG2 '"CQ CQ DE " MY_CALLSIGN " +"'  // Remove/add your Area prefit
//#define CW_MSG3 MY_CALLSIGN

//#define NR_FIR 1  // GW8RDI mod. Usually this won´t fit with CAT, but removing other options, such as CW messages, etc., can make enough space

///G8RDI comment out FAST_AGC & DIAG below to save mem space for CAT
//#define DIAG             1   // Hardware diagnostics on startup (use to debug problems)

#ifndef CAT_XO_CMD  // Undefined CW_VOLUME to make space for CAT_XO_CMD
#define CW_VOLUME        1    // Enable separate CW tone volume in the menu
#endif

#define CW_DECODER       1   // CW decoder
//#define CW_INTERMEDIATE  1   // CW decoder shows intermediate characters (only available for LCD and F_MCU at 20M), sequences like:  EIS[HV] EIUF EAW[JP] EARL TMO TMG[ZQ] TND[BX] TNK[YC], may be good to learn CW; a full list of possible sequences:  EISH5 EISV3 EIUF EIUU2 EAWJ1 EAWP EARL TMOO0 TMOO9 TMOO8 TMGZ7 TMGQ TNDB6 TNDX TNKY TNKC
//#define CW_FREQS_QRP   1   // Defaults to CW QRP   frequencies when changing bands
//#define CW_FREQS_FISTS 1   // Defaults to CW FISTS frequencies when changing bands

// NOTE: Make sure you have the correct xtal frequency enabled. This is the xtal near the SI5351/SI3253 chip, not the one near the Atmega MCU.
//#define F_XTAL    27005000   // 27MHz SI5351 crystal
//#define F_XTAL  25004000   // 25MHz SI5351 crystal  (enable for WB2CBA-uSDX, SI5351 break-out board or uSDXDuO)
#if defined(RED_CORNERS) || defined(BLACK_BRICK)
//#ifdef RED_CORNERS
#define F_XTAL  25000000   // 25MHz SI5351 crystal  (enable for 25MHz TCXO)
#else
#ifdef MY_RED_CORNERS
#define F_XTAL  27001400
#else
#define F_XTAL  27000000   // !!!! SET YOUR EXACT XTAL FREQ OR 27000000 !!!!  27MHz usually on black bricks, Red Buttons (27001400 is my calibration offset!!!) and White buttons versions
#endif
#endif

////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 18/01/2026 - 9M2GRC
// Define frequency oscillator, please tune it based on beat frequency reference 
// For (tr)uSDX, I tune it to this XTAL 
////////////////////////////////////////////////////////////////////////
#ifdef TRUSDX
#define F_XTAL  27001850
#endif // TRUSDX

// GW8RDI NOTE: Enable to have battery voltage shown on the LCD.
// GW8RDI WARNING!!! The problem with the original code is that it switches the ADC VREF up to 5V to read the bat, V, this causes some noise on the IQ sampling which enters the audio,
//  and can interfere with the reading of buttons which are sensed through an ADC.
// A better solution is to simply use a 2 resistor voltage divider and not change VREF, calculating the bar. voltage based on the res. divider ration. Alternatively, only show voltage in a menu function.  todo - change code.
//#define VSS_METER      1   // Supports Vss measurement (as s-meter option), requires resistor of 1M between 12V and pin 26 (PC3)

//#define QCX            1   // Supports older (non-SDR) QCX HW modifications (QCX, QCX-SSB, QCX-DSP with I/Q alignment-feature)
//#define OLED_SSD1306   1   // OLED display (SSD1306 128x32 or 128x64), connect SDA (PD2), SCL (PD3)
//#define OLED_SH1106    1   // OLED display (SH1106 1.3" inch display), connect SDA (PD2), SCL (PD3), NOTE that this display is pretty slow
//#define LCD_I2C        1   // LCD with I2C (PCF8574 module          ), connect SDA (PD2), SCL (PD3), NOTE that this display is pretty slow
//#define LPF_SWITCHING_DL2MAN_USDX_REV3           1   // Enable 8-band filter bank switching:     latching relays wired to a TCA/PCA9555 GPIO extender on the PC4/PC5 I2C bus; relays are using IO0.0 as common (ground), IO1.0..7 used by the individual latches K0-7 switching respectively LPFs for 10m, 15m, 17m, 20m, 30m, 40m, 60m, 80m
//#define LPF_SWITCHING_DL2MAN_USDX_REV3_NOLATCH 1   // Enable 8-band filter bank switching: non-latching relays wired to a TCA/PCA9555 GPIO extender on the PC4/PC5 I2C bus; relays are using IO0.0 as common (ground), IO1.0..7 used by the individual latches K0-7 switching respectively LPFs for 10m, 15m, 17m, 20m, 30m, 40m, 60m, 80m. Enable this if you are using 8-band non-latching version for the relays, the radio will draw extra 15mA current but will work ity any relay (Tnx OH2UDS/TA7W Baris)
//#define LPF_SWITCHING_DL2MAN_USDX_REV2         1   // Enable 5-band filter bank switching:     latching relays wired to a TCA/PCA9555 GPIO extender on the PC4/PC5 I2C bus; relays are using IO0.1 as common (ground), IO0.3, IO0.5, IO0.7, IO1.1, IO1.3 used by the individual latches K1-5 switching respectively LPFs for 20m, 30m, 40m, 60m, 80m
//#define LPF_SWITCHING_DL2MAN_USDX_REV2_BETA    1   // Enable 5-band filter bank switching:     latching relays wired to a PCA9539PW   GPIO extender on the PC4/PC5 I2C bus; relays are using IO0.1 as common (ground), IO0.3, IO0.5, IO0.7, IO1.1, IO1.3 used by the individual latches K1-5 switching respectively LPFs for 20m, 30m, 40m, 60m, 80m
//#define LPF_SWITCHING_DL2MAN_USDX_REV1         1   // Enable 3-band filter bank switching:     latching relays wired to a PCA9536D    GPIO extender on the PC4/PC5 I2C bus; relays are using IO0 as common (ground), IO1-IO3 used by the individual latches K1-3 switching respectively LPFs for 20m, 40m, 80m
//#define LPF_SWITCHING_WB2CBA_USDX_OCTOBAND     1   // Enable 8-band filter bank switching: non-latching relays wired to a MCP23008    GPIO extender on the PC4/PC5 I2C bus; relays are using GND as common (ground), GP0..7 used by the individual latches K1-8 switching respectively LPFs for 80m, 60m, 40m, 30m, 20m, 17m, 15m, 10m
//#define LPF_SWITCHING_PE1DDA_USDXDUO           14  // Enable 2-band filter bank switching: non-latching relay  wired to pin PD5 (pin 11); specify as value the frequency in MHz for which (and above) the relay should be altered (e.g. put 14 to enable the relay at 14MHz and above to use the 20m LPF).
#define SI5351_ADDR   0x60   // SI5351A I2C address: 0x60 for SI5351A-B-GT, Si5351A-B04771-GT, MS5351M; 0x62 for SI5351A-B-04486-GT; 0x6F for SI5351A-B02075-GT; see here for other variants: https://www.silabs.com/TimingUtility/timing-download-document.aspx?OPN=Si5351A-B02075-GT&OPNRevision=0&FileType=PublicAddendum
//#define F_MCU   16000000   // 16MHz ATMEGA328P crystal (enable for unmodified Arduino Uno/Nano boards with 16MHz crystal). You may change this value to any other crystal frequency (up to 28MHz may work)

// Advanced configuration switches
//#define CONDENSED      1   // Display in 4 line mode (for OLED and LCD2004 modules)
#define TX_ENABLE        1   // Disable this for RX only (no transmit), e.g. to support uSDX for kids idea: https://groups.io/g/ucx/topic/81030243#6276
#define SEMI_QSK         1   // Just after keying the transmitter, keeps the RX muted for a short amount of time in the anticipation for continued keying
#define RIT_ENABLE       1   // Receive-In-Transit alternates the receiving frequency with an user-defined offset to compensate for any necessary tuning needed on receive
#define VOX_ENABLE       1   // Voice-On-Xmit which is switching the transceiver into transmit as soon audio is detected (above noise gate level)
//#define MOX_ENABLE     1   // Monitor-On-Xmit which is audio monitoring on speaker during transmit

//#define ONEBUTTON      1   // Use single (encoder) button to control full the rig; optionally use L/R buttons to completely replace rotory encoder function
//#define DEBUG          1   // for development purposes only (adds debugging features such as CPU, sample-rate measurement, additional parameters)
//#define TESTBENCH      1   // Tests RX chain by injection of sine wave, measurements results are sent over serial

// G8RDI removed for memory due to CAT  
//#define TX_DELAY       1   // Enables a delay in the actual transmission to allow relay-switching to be completed before the power is applied (see also NTX, PTX definitions below for GPIO that can switch relay/PA)
//#define NTX            11  // Enables LOW  on TX, used as PTT out to enable external PAs (a value of 11 means PB3 is used)
#define PTX            11  // Enables HIGH on TX, used as PTT out to enable external PAs (a value of 11 means PB3 is used)
//#define CLOCK          1   // Enables clock
// G8RDI removed to save memory #define:
//#define F_XTAL  20000000   // Enable this for uSDXDuO, 20MHz SI5351 crystal
//#define TX_CLK0_CLK1   1   // Enable this for uSDXDuO, i.e. when PA is driven by CLK0, CLK1 (not CLK2); NTX pin may be used for enabling the TX path (this is like RX pin, except that RX may also be used as attenuator)
//#define F_CLK2  12000000   // Enables a fixed CLK2 clock output of choice (only applicable when TX_CLK0_CLK1 is enabled), e.g. for up-converter or to clock UART USB device

// QCX pin defintions
#define LCD_D4  0         //PD0    (pin 2)
#define LCD_D5  1         //PD1    (pin 3)
#define LCD_D6  2         //PD2    (pin 4)
#define LCD_D7  3         //PD3    (pin 5)
#define LCD_EN  4         //PD4    (pin 6)
#define FREQCNT 5         //PD5    (pin 11)
#define ROT_A   6         //PD6    (pin 12)
#define ROT_B   7         //PD7    (pin 13)
#define RX      8         //PB0    (pin 14)
#define SIDETONE 9        //PB1    (pin 15)
#define KEY_OUT 10        //PB2    (pin 16)
#define SIG_OUT 11        //PB3    (pin 17)
#define DAH     12        //PB4    (pin 18)
#define DIT     13        //PB5    (pin 19)
#define AUDIO1  14        //PC0/A0 (pin 23)
#define AUDIO2  15        //PC1/A1 (pin 24)
#define DVM     16        //PC2/A2 (pin 25)
#define BUTTONS 17        //PC3/A3 (pin 26)
#define LCD_RS  18        //PC4    (pin 27)
#define SDA     18        //PC4    (pin 27)
#define SCL     19        //PC5    (pin 28)
//#define NTX   11        //PB3    (pin 17)
//#define PTX   11        //PB3    (pin 17)

#ifdef SWAP_ROTARY
#undef ROT_A
#undef ROT_B
#define ROT_A   7         //PD7    (pin 13)
#define ROT_B   6         //PD6    (pin 12)
#endif

#if (defined(OLED_SSD1306) || defined(OLED_SH1106))
#define OLED    1
#endif

#if (defined(CAT) || defined(TESTBENCH)) && !(OLED)
#define _SERIAL  1       // Coexistence support for serial port and LCD on the same pins
#endif

#ifdef LPF_SWITCHING_DL2MAN_USDX_REV3_NOLATCH
#define LPF_SWITCHING_DL2MAN_USDX_REV3         1
#endif

#ifdef TX_CLK0_CLK1
#ifdef F_CLK2
#define TX1RX0  0b11111000
#define TX1RX1  0b11111000
#define TX0RX1  0b11111000
#define TX0RX0  0b11111011
#else //!F_CLK2
#define TX1RX0  0b11111100
#define TX1RX1  0b11111100
#define TX0RX1  0b11111100
#define TX0RX0  0b11111111
#endif //F_CLK2
#else  //!TX_CLK0_CLK1
#define TX1RX0  0b11111011
#define TX1RX1  0b11111000
#define TX0RX1  0b11111100
#define TX0RX0  0b11111111
#endif //TX_CLK0_CLK1

#if defined(F_CLK2) && !defined(TX_CLK0_CLK1)
#error "TX_CLK0_CLK1 must be enabled in order to use F_CLK2."
#endif

#ifndef TX_ENABLE
#undef KEYER
#undef TX_DELAY
#undef SEMI_QSK
#undef RIT_ENABLE
#undef VOX_ENABLE
#undef MOX_ENABLE
#endif //!TX_ENABLE

#ifdef SWR_METER
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 03/02/2026 - 9M2GRC
// Analog input option for (tr)uSDX, with original version, the analog port is cross
// Using a fixed-point arithmetic, with the unsigned integer variable to save program memory 
////////////////////////////////////////////////////////////////////////
#if defined(TRUSDX) || defined(RED_CORNERS) || defined(BLACK_BRICK)
uint32_t FWD;
uint32_t SWR;
#ifdef TRUSDX
uint32_t ref_V = 5;
#define PIN_FWD  A7
#define PIN_REF  A6
#else
// Use fixed integer variable 
#ifdef BLACK_BRICK
uint32_t ref_V = 5;
// 5.75V represented as 575 (scaled by 100)
#else
const uint16_t ref_V_scaled = 575;
#endif // BLACK_BRICK 
#define PIN_FWD  A6
#define PIN_REF  A7
#endif // TRUSDX
#else 
float FWD;
float SWR;

float ref_V = 5 * 1.15;
#define PIN_FWD  A6
#define PIN_REF  A7
#endif // TRUSDX, RED_CORNERS, BLACK_BRICK
static uint32_t stimer;
#endif // SWR_METER

//FUSES = { .low = 0xFF, .high = 0xD6, .extended = 0xFD };   // Fuse settings should be set at programming (Arduino IDE > Tools > Burn bootloader)

#if(ARDUINO < 10810)
#if (ARDUINO != 10607)  // G8RDI mod - IDE 2.0.1 has its version set to 10607, an IDE bug since fixed.
# error "Unsupported Arduino IDE version, use Arduino IDE 1.8.10 or later from https://www.arduino.cc/en/software"
#endif
#endif

#if !(defined(ARDUINO_ARCH_AVR))
#error "Unsupported architecture, select Arduino IDE > Tools > Board > Arduino AVR Boards > Arduino Uno."
#endif

#if(F_CPU != 16000000)
#error "Unsupported clock frequency, Arduino IDE must specify 16MHz clock; alternate crystal frequencies may be specified with F_MCU."
#endif

#undef F_CPU
#define F_CPU 20007000  // Actual crystal frequency of 20MHz XTAL1, note that this declaration is just informative and does not correct the timing in Arduino functions like delay(); hence a 1.25 factor needs to be added for correction.
#ifndef F_MCU
#define F_MCU 20000000  // 20MHz ATMEGA328P crystal
#endif

extern char __bss_end;
static int freeMemory() { char* sp = reinterpret_cast<char*>(SP); return sp - &__bss_end; }  // see: http://www.nongnu.org/avr-libc/user-manual/malloc.html

#ifdef CAT_EXT
volatile uint8_t cat_key = 0;
uint8_t _digitalRead(uint8_t pin) {  // reads pin or (via CAT) artificially overriden pins
	serialEvent();  // allows CAT update
	if (cat_key) { return (pin == BUTTONS) ? ((cat_key & 0x07) > 0) : (pin == DIT) ? ~cat_key & 0x10 : (pin == DAH) ? ~cat_key & 0x20 : 0; } // overrides digitalRead(DIT, DAH, BUTTONS);
	return digitalRead(pin);
}
#else
#define _digitalRead(x) digitalRead(x)
#endif //CAT_EXT

//#define ONEBUTTON_INV 1 // Encoder button goes from PC3 to GND (instead PC3 to 5V, with 10k pull down)
#ifdef ONEBUTTON_INV
uint8_t inv = 1;
#else
uint8_t inv = 0;
#endif

// keyerControl bit definitions
#define DIT_L    0x01     // Dit latch
#define DAH_L    0x02     // Dah latch
#define DIT_PROC 0x04     // Dit is being processed
#define PDLSWAP  0x08     // 0 for normal, 1 for swap
#define IAMBICB  0x10     // 0 for Iambic A, 1 for Iambic B
#define IAMBICA  0x00     // 0 for Iambic A, 1 for Iambic B
#define SINGLE   2        // Keyer Mode 0 1 -> Iambic2  2 ->SINGLE

int keyer_speed = 25;
static unsigned long ditTime;                    // No. milliseconds per dit
static uint8_t keyerControl;
static uint8_t keyerState;
static uint8_t keyer_mode = 2; //->  SINGLE
static uint8_t keyer_swap = 0; //->  DI/DAH

static uint32_t ktimer;
static int Key_state;
int debounce;

enum KSTYPE { IDLE, CHK_DIT, CHK_DAH, KEYED_PREP, KEYED, INTER_ELEMENT }; // State machine states

void update_PaddleLatch() // Latch dit and/or dah press, called by keyer routine
{
	if (_digitalRead(DIT) == LOW) {
		keyerControl |= keyer_swap ? DAH_L : DIT_L;
	}
	if (_digitalRead(DAH) == LOW) {
		keyerControl |= keyer_swap ? DIT_L : DAH_L;
	}
}

void loadWPM(int wpm) // Calculate new time constants based on wpm value
{
#if(F_MCU != 20000000)
	ditTime = (1200ULL * F_MCU / 16000000) / wpm;   //ditTime = 1200/wpm;  compensated for F_CPU clock (running in a 16MHz Arduino environment)
#else
	ditTime = (1200 * 5 / 4) / wpm;   //ditTime = 1200/wpm;  compensated for 20MHz clock (running in a 16MHz Arduino environment)
#endif
}
static uint8_t practice = false;  // Practice mode

static int8_t prev_mode;

volatile uint8_t cat_active = 0;  // Run-time set when serial data being processed to keep shared LCD pins controlled
volatile uint32_t rxend_event = 0;
volatile uint8_t vox = 0;

#include <avr/sleep.h>
#include <avr/wdt.h>

//#define _I2C_DIRECT_IO    1 // Enables communications that is not using the standard I/O pull-down approach with pull-up resistors, instead I/O is directly driven with 0V/5V
class I2C_ { // Secundairy I2C class used by I2C LCD/OLED, uses alternate pins: PD2 (SDA) and PD3 (SCL)
public:
#if(F_MCU > 20900000)
#ifdef OLED_SH1106
#define _DELAY() for(uint8_t i = 0; i != 9; i++) asm("nop");
#else
#ifdef OLED_SSD1306
#define _DELAY() for(uint8_t i = 0; i != 6; i++) asm("nop");
#else // other (I2C_LCD)
#define _DELAY() for(uint8_t i = 0; i != 7; i++) asm("nop");
#endif
#endif
#else // slow F_MCU
#ifdef OLED_SH1106
#define _DELAY() for(uint8_t i = 0; i != 8; i++) asm("nop");
#else
#ifdef OLED_SSD1306
#define _DELAY() for(uint8_t i = 0; i != 4; i++) asm("nop"); // 4=731kb/s
#else // other (I2C_LCD)
#define _DELAY() for(uint8_t i = 0; i != 5; i++) asm("nop");
#endif
#endif
#endif // F_MCU
#define _I2C_SDA (1<<2) // PD2
#define _I2C_SCL (1<<3) // PD3
#ifdef _I2C_DIRECT_IO
#define _I2C_INIT() _I2C_SDA_HI(); _I2C_SCL_HI(); DDRD |= (_I2C_SDA | _I2C_SCL);  // direct I/O (no need for pull-ups)
#define _I2C_SDA_HI() PORTD |=  _I2C_SDA;
#define _I2C_SDA_LO() PORTD &= ~_I2C_SDA;
#define _I2C_SCL_HI() PORTD |=  _I2C_SCL; _DELAY();
#define _I2C_SCL_LO() PORTD &= ~_I2C_SCL; _DELAY();
#else // !_I2C_DIRECT_IO
#define _I2C_INIT()   PORTD &= ~_I2C_SDA; PORTD &= ~_I2C_SCL; _I2C_SDA_HI(); _I2C_SCL_HI();  // open-drain
#define _I2C_SDA_HI() DDRD &= ~_I2C_SDA;
#define _I2C_SDA_LO() DDRD |=  _I2C_SDA;
#define _I2C_SCL_HI() DDRD &= ~_I2C_SCL; _DELAY();
#define _I2C_SCL_LO() DDRD |=  _I2C_SCL; _DELAY();
#endif // !_I2C_DIRECT_IO
#define _I2C_START() _I2C_SDA_LO(); _DELAY(); _I2C_SCL_LO(); // _I2C_SDA_HI();
#define _I2C_STOP()  _I2C_SDA_LO(); _I2C_SCL_HI(); _I2C_SDA_HI();
#define _I2C_SUSPEND() //_I2C_SDA_LO(); // SDA_LO to allow re-use as output port
#define _SendBit(data, bit) \
    if(data & 1 << bit){ \
      _I2C_SDA_HI();  \
    } else {         \
      _I2C_SDA_LO();  \
    }                \
    _I2C_SCL_HI();    \
    _I2C_SCL_LO();
	inline void start() { _I2C_INIT(); _I2C_START(); };
	inline void stop() { _I2C_STOP(); _I2C_SUSPEND(); };
	inline void SendByte(uint8_t data) {
		_SendBit(data, 7);
		_SendBit(data, 6);
		_SendBit(data, 5);
		_SendBit(data, 4);
		_SendBit(data, 3);
		_SendBit(data, 2);
		_SendBit(data, 1);
		_SendBit(data, 0);
		_I2C_SDA_HI();  // recv ACK
		_DELAY(); //
		_I2C_SCL_HI();
		_I2C_SCL_LO();
	}
	void SendRegister(uint8_t addr, uint8_t* data, uint8_t n) {
		start();
		SendByte(addr << 1);
		while (n--) SendByte(*data++);
		stop();
	}

	void begin() {};
	void beginTransmission(uint8_t addr) { start(); SendByte(addr << 1); };
	bool write(uint8_t byte) { SendByte(byte); return 1; };
	uint8_t endTransmission() { stop(); return 0; };
};
I2C_ Wire;

uint8_t backlight = 8;
//#define RS_HIGH_ON_IDLE   1   // Experimental LCD support where RS line is high on idle periods to comply with SDA I2C standard.

class LCD : public Print {  // inspired by: http://www.technoblogy.com/show?2BET
public:  // LCD1602 display in 4-bit mode, RS is pull-up and kept low when idle to prevent potential display RFI via RS line
#define _dn  0      // PD0 to PD3 connect to D4 to D7 on the display
#define _en  4      // PD4 - MUST have pull-up resistor
#define _rs  4      // PC4 - MUST have pull-up resistor
#define RS_PULLUP  1   // Use pullup on RS line, ensures compliancy to the absolute maximum ratings for the si5351 sda input that is shared with rs pin of lcd
#ifdef RS_PULLUP
#define LCD_RS_HI() DDRC &= ~(1 << _rs); asm("nop"); // RS high (pull-up)
#define LCD_RS_LO() DDRC |= 1 << _rs;                // RS low (pull-down)
#else
#define LCD_RS_LO() PORTC &= ~(1 << _rs);        // RS low
#define LCD_RS_HI() PORTC |= (1 << _rs);         // RS high
#endif //RS_PULLUP
#define LCD_EN_LO() PORTD &= ~(1 << _en);        // EN low
#define LCD_EN_HI() PORTD |= (1 << _en);         // EN high
#define LCD_PREP_NIBBLE(b) (PORTD & ~(0xf << _dn)) | (b) << _dn | 1 << _en // Send data and enable high
	uint8_t _cols;
	void begin(uint8_t x = 0, uint8_t y = 0) {        // Send command , make sure at least 40ms after power-up before sending commands
#ifdef LCD_I2C
#define PCF_ADDR  0x27                         // LCD I2C address where PCF8574 addess selection A0, A1, A2 are all open
#define PCF_RS 0x01
#define PCF_RW 0x02  // the 0xF0 bits are used for 4-bit data to the display.
#define PCF_EN 0x04
#define PCF_BACKLIGHT 0x08
		Wire.beginTransmission(PCF_ADDR);
		Wire.write(0);
		Wire.endTransmission();
		delayMicroseconds(50000);
#else //!LCD_I2C
		DDRD |= 0xf << _dn | 1 << _en;                 // Make data, EN outputs
		DDRC |= 1 << _rs;
		delayMicroseconds(50000);                      // *
		LCD_RS_LO(); LCD_EN_LO();
#endif //!LCD_I2C
		cmd(0x33);                                     // Ensures display is in 8-bit mode
		delayMicroseconds(4500); cmd(0x33); delayMicroseconds(4500); cmd(0x33); delayMicroseconds(150); // * Ensures display is in 8-bit mode
		cmd(0x32);                                     // Puts display in 4-bit mode
		cmd(0x28);                                     // * Function set: 2-line, 5x8 
		cmd(0x0c);                                     // Display on
		cmd(0x01);                                     // Clear display
		delay(3);                                      // Allow to execute Clear on display [https://www.sparkfun.com/datasheets/LCD/HD44780.pdf, p.49, p58]
		cmd(0x06);                                     // * Entrymode: left, shift-dec
	}
#ifdef LCD_I2C
	void nib(uint8_t b, bool isData) {                             // Send four bit nibble to display
		b = (b << 4) | ((backlight) ? PCF_BACKLIGHT : 0) | ((isData) ? PCF_RS : 0);
		Wire.write(b | PCF_EN);  // write command EN HI
		delayMicroseconds(4);    // enable pulse must be >450ns
		Wire.write(b);           // write command EN LO
		delayMicroseconds(60);   // commands need > 37us to settle
		Wire.write(b);           // must write for some unknown reason
	}
	void cmd(uint8_t b) {
		Wire.beginTransmission(PCF_ADDR);
		nib(b >> 4, false); nib(b, false);
		Wire.endTransmission();
	}
	size_t write(uint8_t b) {
		Wire.beginTransmission(PCF_ADDR);
		nib((b >> 4), true); nib((b), true);
		Wire.endTransmission();
	}
#else //!LCD_I2C
	void pre() {
#ifdef _SERIAL
		if (!vox) if (cat_active) { Serial.flush(); for (; millis() < rxend_event;)wdt_reset(); PORTC |= 1 << 2; DDRC |= 1 << 2; } UCSR0B &= ~((1 << RXEN0) | (1 << TXEN0)); // Complete serial TX and RX; mask PD1 LCD data-exchange by pulling-up TXD via PC2 HIGH; enable PD0/PD1, disable serial port
#endif
		noInterrupts();  // ***!!!*** do not allow LCD tranfer to be interrupted, to prevent backlight to lighten-up
	}
	void post() {
		if (backlight)
			PORTD |= BACKLIGHT_PIN;
		else
			PORTD &= ~BACKLIGHT_PIN;   // Backlight control - G8RDI MOD
#ifdef _SERIAL
		UCSR0B |= (1 << RXEN0) | (1 << TXEN0); if (!vox) if (cat_active) { PORTC &= ~(1 << 2); } // Enable serial port, disable PD0, PD1; PC2 LOW to prevent CAT TX disruption via MIC input
#endif
		interrupts();
	}
#ifdef RS_HIGH_ON_IDLE
	void cmd(uint8_t b) {
		pre();
		uint8_t nibh = LCD_PREP_NIBBLE(b >> 4);       // Prepare high nibble data and enable high
		PORTD = nibh;                                  // Send high nibble data and enable high
		uint8_t nibl = LCD_PREP_NIBBLE(b & 0xf);       // Prepare low nibble data and enable high
		LCD_RS_LO();
		LCD_EN_LO();
		PORTD = nibl;                                  // Send low nibble data and enable high
		asm("nop"); asm("nop");                        // Keep RS low, but complete enable cycle (should be 500ns)
		LCD_EN_LO();
		LCD_RS_HI();
		post();
		delayMicroseconds(60);                         // Execution time  (37+4)*1.25 us
	}
	size_t write(uint8_t b) {                         // Write data:    send nibbles while RS high
		pre();
		uint8_t nibh = LCD_PREP_NIBBLE(b >> 4);       // Prepare high nibble data and enable high
		PORTD = nibh;                                  // Send high nibble data and enable high
		uint8_t nibl = LCD_PREP_NIBBLE(b & 0xf);       // Prepare low nibble data and enable high
		LCD_RS_HI();
		LCD_EN_LO();
		PORTD = nibl;                                  // Send low nibble data and enable high
		asm("nop"); asm("nop");                        // Keep RS high, but complete enable cycle (should be 500ns)
		LCD_EN_LO();
		post();
		delayMicroseconds(60);                         // Execution time  (37+4)*1.25 us
		return 1;
	}
#else //!RS_HIGH_ON_IDLE
	void nib(uint8_t b) {                             // Send four bit nibble to display
		pre();
		PORTD = LCD_PREP_NIBBLE(b);                    // Send data and enable high
		delayMicroseconds(4);
		LCD_EN_LO();
		post();
		delayMicroseconds(60);                         // Execution time
	}
	void cmd(uint8_t b) { nib(b >> 4); nib(b & 0xf); } // Write command: send nibbles while RS low
	size_t write(uint8_t b) {                         // Write data:    send nibbles while RS high
		pre();
		uint8_t nibh = LCD_PREP_NIBBLE(b >> 4);       // Prepare high nibble data and enable high
		PORTD = nibh;                                  // Send high nibble data and enable high
		uint8_t nibl = LCD_PREP_NIBBLE(b & 0xf);       // Prepare low nibble data and enable high
		LCD_RS_HI();
		LCD_EN_LO();
		PORTD = nibl;                                  // Send low nibble data and enable high
		LCD_RS_LO();
		LCD_RS_HI();
		LCD_EN_LO();
		LCD_RS_LO();
		post();
		delayMicroseconds(60);                         // Execution time  (37+4)*1.25 us
		return 1;
	}
#endif // RS_HIGH_ON_IDLE
#endif //!LCD_I2C
#ifdef CONDENSED
	void setCursor(uint8_t x, uint8_t y) { cmd(0x80 | (x + (uint8_t[]) { 0x00, 0x40, 0x00 + 20, 0x40 + 20 } [y])); }  // ONLY for LCD2004 display
#else
	void setCursor(uint8_t x, uint8_t y) { cmd(0x80 | (x + y * 0x40)); }
#endif
	void cursor() { cmd(0x0e); }
	void noCursor() { cmd(0x0c); }
	void noDisplay() { cmd(0x08); }
	void createChar(uint8_t l, uint8_t glyph[]) { cmd(0x40 | ((l & 0x7) << 3)); for (int i = 0; i != 8; i++) write(glyph[i]); }
};

#ifndef REMOVEFONT   // G8RDI mod - FONT NOT COMPILED IF NOT USED
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 18/01/2026 - 9M2GRC
// To saved program memory during code compilation, removed unnecessary character 
////////////////////////////////////////////////////////////////////////
# ifdef TRUSDX
const uint8_t font[] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [0] Space
   0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, // [1] .
   0x00, 0x08, 0x08, 0x3e, 0x3e, 0x08, 0x08, 0x00, // [2] + 
   0x00, 0x41, 0x41, 0x63, 0x36, 0x1c, 0x08, 0x00, // [3] >
   0x00, 0x00, 0x80, 0xe0, 0x60, 0x00, 0x00, 0x00, // [4] ,
   0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, // [5] -
   0x00, 0x3e, 0x7f, 0x49, 0x45, 0x7f, 0x3e, 0x00, // [6] 0-9 starts here...
   0x00, 0x40, 0x44, 0x7f, 0x7f, 0x40, 0x40, 0x00, // [7] 1
   0x00, 0x62, 0x73, 0x51, 0x49, 0x4f, 0x46, 0x00, // [8] 2
   0x00, 0x22, 0x63, 0x49, 0x49, 0x7f, 0x36, 0x00, // [9] 3
   0x00, 0x18, 0x18, 0x14, 0x16, 0x7f, 0x7f, 0x10, // [10] 4
   0x00, 0x27, 0x67, 0x45, 0x45, 0x7d, 0x39, 0x00, // [11] 5
   0x00, 0x3e, 0x7f, 0x49, 0x49, 0x7b, 0x32, 0x00, // [12] 6
   0x00, 0x03, 0x03, 0x79, 0x7d, 0x07, 0x03, 0x00, // [13] 7
   0x00, 0x36, 0x7f, 0x49, 0x49, 0x7f, 0x36, 0x00, // [14] 8
   0x00, 0x26, 0x6f, 0x49, 0x49, 0x7f, 0x3e, 0x00, // [15] 9
   0x00, 0x7c, 0x7e, 0x0b, 0x0b, 0x7e, 0x7c, 0x00, // [16] A-Z starts here...
   0x00, 0x7f, 0x7f, 0x49, 0x49, 0x7f, 0x36, 0x00, // [17] B
   0x00, 0x3e, 0x7f, 0x41, 0x41, 0x63, 0x22, 0x00, // [18] C
   0x00, 0x7f, 0x7f, 0x41, 0x63, 0x3e, 0x1c, 0x00, // [19] D
   0x00, 0x7f, 0x7f, 0x49, 0x49, 0x41, 0x41, 0x00, // [20] E
   0x00, 0x7f, 0x7f, 0x09, 0x09, 0x01, 0x01, 0x00, // [21] F
   0x00, 0x3e, 0x7f, 0x41, 0x49, 0x7b, 0x3a, 0x00, // [22] G
   0x00, 0x7f, 0x7f, 0x08, 0x08, 0x7f, 0x7f, 0x00, // [23] H
   0x00, 0x00, 0x41, 0x7f, 0x7f, 0x41, 0x00, 0x00, // [24] I
   0x00, 0x20, 0x60, 0x41, 0x7f, 0x3f, 0x01, 0x00, // [25] J
   0x00, 0x7f, 0x7f, 0x1c, 0x36, 0x63, 0x41, 0x00, // [26] K
   0x00, 0x7f, 0x7f, 0x40, 0x40, 0x40, 0x40, 0x00, // [27] L
   0x00, 0x7f, 0x7f, 0x06, 0x0c, 0x06, 0x7f, 0x7f, // [28] M
   0x00, 0x7f, 0x7f, 0x0e, 0x1c, 0x7f, 0x7f, 0x00, // [29] N
   0x00, 0x3e, 0x7f, 0x41, 0x41, 0x7f, 0x3e, 0x00, // [30] O
   0x00, 0x7f, 0x7f, 0x09, 0x09, 0x0f, 0x06, 0x00, // [31] P
   0x00, 0x1e, 0x3f, 0x21, 0x61, 0x7f, 0x5e, 0x00, // [32] Q
   0x00, 0x7f, 0x7f, 0x19, 0x39, 0x6f, 0x46, 0x00, // [33] R
   0x00, 0x26, 0x6f, 0x49, 0x49, 0x7b, 0x32, 0x00, // [34] S
   0x00, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01, 0x00, // [35] T
   0x00, 0x3f, 0x7f, 0x40, 0x40, 0x7f, 0x3f, 0x00, // [36] U
   0x00, 0x1f, 0x3f, 0x60, 0x60, 0x3f, 0x1f, 0x00, // [37] V
   0x00, 0x7f, 0x7f, 0x30, 0x18, 0x30, 0x7f, 0x7f, // [38] W
   0x00, 0x63, 0x77, 0x1c, 0x1c, 0x77, 0x63, 0x00, // [39] X
   0x00, 0x07, 0x0f, 0x78, 0x78, 0x0f, 0x07, 0x00, // [40] Y
   0x00, 0x61, 0x71, 0x59, 0x4d, 0x47, 0x43, 0x00, // [41] Z
   // Custom Icons start here...
   
   0x00, 0x55, 0x2A, 0x2A, 0x14, 0x14, 0x08, 0x08, // [42] Logo
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [43] S0
   0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, // [44] S1
   0x00, 0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00, // [45] S2
   0x00, 0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, // [46] S3
   0x1C, 0x1E, 0x05, 0x05, 0x1E, 0x1C, 0x00, 0x00, // [47] VFO-A
   0x1F, 0x1F, 0x15, 0x15, 0x0A, 0x0A, 0x00, 0x00  // [48] VFO-B
   
};
#else 
// C64 real
//G8RDI mod to free more prog memory (FONT NOT COMPILED IF NOT USED) //const uint8_t font[]PROGMEM = {
const uint8_t font[]PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ' '
   0x00, 0x00, 0x00, 0x4f, 0x4f, 0x00, 0x00, 0x00, // !
   0x00, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x00, // "
   0x14, 0x7f, 0x7f, 0x14, 0x14, 0x7f, 0x7f, 0x14, // #
   0x00, 0x24, 0x2e, 0x6b, 0x6b, 0x3a, 0x12, 0x00, // $
   0x00, 0x63, 0x33, 0x18, 0x0c, 0x66, 0x63, 0x00, // %
   0x00, 0x32, 0x7f, 0x4d, 0x4d, 0x77, 0x72, 0x50, // &
   0x00, 0x00, 0x00, 0x04, 0x06, 0x03, 0x01, 0x00, // '
   0x00, 0x00, 0x1c, 0x3e, 0x63, 0x41, 0x00, 0x00, // (
   0x00, 0x00, 0x41, 0x63, 0x3e, 0x1c, 0x00, 0x00, // )
   0x08, 0x2a, 0x3e, 0x1c, 0x1c, 0x3e, 0x2a, 0x08, // *
   0x00, 0x08, 0x08, 0x3e, 0x3e, 0x08, 0x08, 0x00, // +
   0x00, 0x00, 0x80, 0xe0, 0x60, 0x00, 0x00, 0x00, // ,
   0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, // -
   0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, // .
   0x00, 0x40, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, // /
   0x00, 0x3e, 0x7f, 0x49, 0x45, 0x7f, 0x3e, 0x00, // 0
   0x00, 0x40, 0x44, 0x7f, 0x7f, 0x40, 0x40, 0x00, // 1
   0x00, 0x62, 0x73, 0x51, 0x49, 0x4f, 0x46, 0x00, // 2
   0x00, 0x22, 0x63, 0x49, 0x49, 0x7f, 0x36, 0x00, // 3
   0x00, 0x18, 0x18, 0x14, 0x16, 0x7f, 0x7f, 0x10, // 4
   0x00, 0x27, 0x67, 0x45, 0x45, 0x7d, 0x39, 0x00, // 5
   0x00, 0x3e, 0x7f, 0x49, 0x49, 0x7b, 0x32, 0x00, // 6
   0x00, 0x03, 0x03, 0x79, 0x7d, 0x07, 0x03, 0x00, // 7
   0x00, 0x36, 0x7f, 0x49, 0x49, 0x7f, 0x36, 0x00, // 8
   0x00, 0x26, 0x6f, 0x49, 0x49, 0x7f, 0x3e, 0x00, // 9
   0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x00, // :
   0x00, 0x00, 0x80, 0xe4, 0x64, 0x00, 0x00, 0x00, // ;
   0x00, 0x08, 0x1c, 0x36, 0x63, 0x41, 0x41, 0x00, // <
   0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, // =
   0x00, 0x41, 0x41, 0x63, 0x36, 0x1c, 0x08, 0x00, // >
   0x00, 0x02, 0x03, 0x51, 0x59, 0x0f, 0x06, 0x00, // ?
   0x00, 0x3e, 0x7f, 0x41, 0x4d, 0x4f, 0x2e, 0x00, // @
   0x00, 0x7c, 0x7e, 0x0b, 0x0b, 0x7e, 0x7c, 0x00, // A
   0x00, 0x7f, 0x7f, 0x49, 0x49, 0x7f, 0x36, 0x00, // B
   0x00, 0x3e, 0x7f, 0x41, 0x41, 0x63, 0x22, 0x00, // C
   0x00, 0x7f, 0x7f, 0x41, 0x63, 0x3e, 0x1c, 0x00, // D
   0x00, 0x7f, 0x7f, 0x49, 0x49, 0x41, 0x41, 0x00, // E
   0x00, 0x7f, 0x7f, 0x09, 0x09, 0x01, 0x01, 0x00, // F
   0x00, 0x3e, 0x7f, 0x41, 0x49, 0x7b, 0x3a, 0x00, // G
   0x00, 0x7f, 0x7f, 0x08, 0x08, 0x7f, 0x7f, 0x00, // H
   0x00, 0x00, 0x41, 0x7f, 0x7f, 0x41, 0x00, 0x00, // I
   0x00, 0x20, 0x60, 0x41, 0x7f, 0x3f, 0x01, 0x00, // J
   0x00, 0x7f, 0x7f, 0x1c, 0x36, 0x63, 0x41, 0x00, // K
   0x00, 0x7f, 0x7f, 0x40, 0x40, 0x40, 0x40, 0x00, // L
   0x00, 0x7f, 0x7f, 0x06, 0x0c, 0x06, 0x7f, 0x7f, // M
   0x00, 0x7f, 0x7f, 0x0e, 0x1c, 0x7f, 0x7f, 0x00, // N
   0x00, 0x3e, 0x7f, 0x41, 0x41, 0x7f, 0x3e, 0x00, // O
   0x00, 0x7f, 0x7f, 0x09, 0x09, 0x0f, 0x06, 0x00, // P
   0x00, 0x1e, 0x3f, 0x21, 0x61, 0x7f, 0x5e, 0x00, // Q
   0x00, 0x7f, 0x7f, 0x19, 0x39, 0x6f, 0x46, 0x00, // R
   0x00, 0x26, 0x6f, 0x49, 0x49, 0x7b, 0x32, 0x00, // S
   0x00, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01, 0x00, // T
   0x00, 0x3f, 0x7f, 0x40, 0x40, 0x7f, 0x3f, 0x00, // U
   0x00, 0x1f, 0x3f, 0x60, 0x60, 0x3f, 0x1f, 0x00, // V
   0x00, 0x7f, 0x7f, 0x30, 0x18, 0x30, 0x7f, 0x7f, // W
   0x00, 0x63, 0x77, 0x1c, 0x1c, 0x77, 0x63, 0x00, // X
   0x00, 0x07, 0x0f, 0x78, 0x78, 0x0f, 0x07, 0x00, // Y
   0x00, 0x61, 0x71, 0x59, 0x4d, 0x47, 0x43, 0x00, // Z
   0x00, 0x00, 0x7f, 0x7f, 0x41, 0x41, 0x00, 0x00, // [
   0x00, 0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x40,
   0x00, 0x00, 0x41, 0x41, 0x7f, 0x7f, 0x00, 0x00, // ]
   0x00, 0x08, 0x0c, 0xfe, 0xfe, 0x0c, 0x08, 0x00, // ^
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // _
   0x00, 0x01, 0x03, 0x06, 0x04, 0x00, 0x00, 0x00, // '
   0x00, 0x20, 0x74, 0x54, 0x54, 0x7c, 0x78, 0x00, // a
   0x00, 0x7e, 0x7e, 0x48, 0x48, 0x78, 0x30, 0x00, // b
   0x00, 0x38, 0x7c, 0x44, 0x44, 0x44, 0x00, 0x00, // c
   0x00, 0x30, 0x78, 0x48, 0x48, 0x7e, 0x7e, 0x00, // d
   0x00, 0x38, 0x7c, 0x54, 0x54, 0x5c, 0x18, 0x00, // e
   0x00, 0x00, 0x08, 0x7c, 0x7e, 0x0a, 0x0a, 0x00, // f
   0x00, 0x98, 0xbc, 0xa4, 0xa4, 0xfc, 0x7c, 0x00, // g
   0x00, 0x7e, 0x7e, 0x08, 0x08, 0x78, 0x70, 0x00, // h
   0x00, 0x00, 0x48, 0x7a, 0x7a, 0x40, 0x00, 0x00, // i
   0x00, 0x00, 0x80, 0x80, 0x80, 0xfa, 0x7a, 0x00, // j
   0x00, 0x7e, 0x7e, 0x10, 0x38, 0x68, 0x40, 0x00, // k
   0x00, 0x00, 0x42, 0x7e, 0x7e, 0x40, 0x00, 0x00, // l
   0x00, 0x7c, 0x7c, 0x18, 0x38, 0x1c, 0x7c, 0x78, // m
   0x00, 0x7c, 0x7c, 0x04, 0x04, 0x7c, 0x78, 0x00, // n
   0x00, 0x38, 0x7c, 0x44, 0x44, 0x7c, 0x38, 0x00, // o
   0x00, 0xfc, 0xfc, 0x24, 0x24, 0x3c, 0x18, 0x00, // p
   0x00, 0x18, 0x3c, 0x24, 0x24, 0xfc, 0xfc, 0x00, // q
   0x00, 0x7c, 0x7c, 0x04, 0x04, 0x0c, 0x08, 0x00, // r
   0x00, 0x48, 0x5c, 0x54, 0x54, 0x74, 0x24, 0x00, // s
   0x00, 0x04, 0x04, 0x3e, 0x7e, 0x44, 0x44, 0x00, // t
   0x00, 0x3c, 0x7c, 0x40, 0x40, 0x7c, 0x7c, 0x00, // u
   0x00, 0x1c, 0x3c, 0x60, 0x60, 0x3c, 0x1c, 0x00, // v
   0x00, 0x1c, 0x7c, 0x70, 0x38, 0x70, 0x7c, 0x1c, // w
   0x00, 0x44, 0x6c, 0x38, 0x38, 0x6c, 0x44, 0x00, // x
   0x00, 0x9c, 0xbc, 0xa0, 0xe0, 0x7c, 0x3c, 0x00, // y
   0x00, 0x44, 0x64, 0x74, 0x5c, 0x4c, 0x44, 0x00, // z
   0x00, 0x08, 0x3e, 0x77, 0x41, 0x41, 0x00, 0x00, // {
   0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, // |
   0x00, 0x00, 0x41, 0x41, 0x77, 0x3e, 0x08, 0x00, // }
   0x00, 0x04, 0x02, 0x02, 0x04, 0x04, 0x02, 0x00, // ~

  0b0000000,  // 126+1; logo
  0b1010101,
  0b0101010,
  0b0101010,
  0b0010100,
  0b0010100,
  0b0001000,
  0b0001000,
  0b00000,  // 126+2; s-meter, 0 bars
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,  // 126+3; s-meter, 1 bars
  0b00000,
  0b00000,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,  // 126+4; s-meter, 2 bars
  0b00000,
  0b00000,
  0b11111,
  0b00000,
  0b11111,
  0b00000,
  0b00000,
  0b00000,  // 126+5; s-meter, 3 bars
  0b00000,
  0b00000,
  0b11111,
  0b00000,
  0b11111,
  0b00000,
  0b11111,
  0b00000,  // 126+6; vfo-a
  0b11100,
  0b11110,
  0b00101,
  0b00101,
  0b11110,
  0b11100,
  0b00000,
  0b00000,  // 126+7; vfo-b
  0b11111,
  0b11111,
  0b10101,
  0b10101,
  0b01010,
  0b01010,
  0b00000 };
#endif // TRUSDX
#endif 

#define FONT_W 8
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 18/01/2026 - 9M2GRC
// To saved program memory during code compilation, OLED font height reduce to 1
////////////////////////////////////////////////////////////////////////
#ifndef TRUSDX 
#define FONT_H 2
#else
#define FONT_H 1	
#endif // TRUSDX
#define FONT_STRETCHV 1
#define FONT_STRETCHH 0

//#define INVERSE  1
static const uint8_t oled_init_sequence[] PROGMEM = {  // Initialization Sequence  https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
	//0xAE,       // Display OFF (sleep mode)
	0xD5, 0x80,   // 0x01--set display clock divide ratio/oscillator frequency   OK? (0x80 (or >=0x10) needed when multiplex ration set to 0x3F)
  #ifdef CONDENSED
	0xA8, 0x3F,   // Set multiplex ratio(1 to 64)   128x64
  #else
	0xA8, 0x1F,   // Set multiplex ratio(1 to 64)   128x32
  #endif
	0xD3, 0x00,   // Set display offset. 00 = no offset
  #ifndef OLED_SH1106  // for SSD1306 only:
	0x40,         // Set display start line address
	0x8D, 0x14,   // Set charge pump, internal VCC
	0x20, 0x02,   // Set Memory Addressing; 0=Horizontal Mode; 1=Vertical Mode; 2=Page Mode
	0xA4,     // Output RAM to Display  (display all on resume)   0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
  #endif //!OLED_SH1106
	0xA1,         // Set Segment Re-map. A0=column 0 mapped to SEG0; A1=column 127 mapped to SEG0. Flip Horizontally
	0xC8,         // Set COM Output Scan Direction.  Flip Veritically.
  #ifdef CONDENSED
	0xDA, 0x12,   // Set com pins hardware configuration  128x64
  #else
	0xDA, 0x02,   // Set com pins hardware configuration  128x32
  #endif
	0x81, 0x80,   // Set contrast control register
	0xDB, 0x40,   // Set vcomh 0x20 = 0.77xVcc
	0xD9, 0xF1,   // 0xF1=brighter //0x22 Set pre-charge period
	0xB0 | 0x0,   // Set page address, 0-7
  #ifdef OLED_SH1106
	0xAD, 0x8B,   // SH1106 Set pump mode: pump ON
	0x30 | 0x2,   // SH1106 Pump voltage 8.0V
  #endif //OLED_SH1106
  #ifdef INVERSE
	0xA7,         // Set display mode: Inverse
  #else
	0xA6,         // Set display mode: Normal
  #endif
	//0x00,       // Set low nibble of column address
	//0x10,       // Set high nibble of column address
	0xAF,         // Display ON
};

class OLEDDevice : public Print {  // https://www.buydisplay.com/download/manual/ER-OLED0.91-3_Series_Datasheet.pdf
public:
#define OLED_ADDR    0x3C  // Slave address
#define OLED_PAGES   4
#define OLED_COMMAND 0x00
#define OLED_DATA    0x40
	uint8_t oledX = 0, oledY = 0;
	uint8_t renderingFrame = 0xB0;
	bool wrap = false;
	
	void cmd(uint8_t b) {
		Wire.beginTransmission(OLED_ADDR); Wire.write(OLED_COMMAND);
		Wire.write(b);
		Wire.endTransmission();
	}
	void begin(uint8_t cols, uint8_t rows, uint8_t charsize = 0) {
		Wire.begin();
		
		Wire.beginTransmission(OLED_ADDR); Wire.write(OLED_COMMAND);
		for (uint8_t i = 0; i < sizeof(oled_init_sequence); i++) {
			Wire.write(pgm_read_byte(&oled_init_sequence[i]));
		}
		Wire.endTransmission();
		delayMicroseconds(100);
#ifdef CONDENSED
		for (uint8_t y = 0; y != rows; y++) for (uint8_t x = 0; x != cols; x++) { setCursor(x, y); write(' '); }  // clear
#endif
	}
	bool curs = false;
	void noCursor() { curs = false; }
	void cursor() { curs = true; }
	void noDisplay() { cmd(0xAE); }
	void createChar(uint8_t l, uint8_t glyph[]) {}
	
	void _setCursor(uint8_t x, uint8_t y) {
		oledX = x; oledY = y;
		Wire.beginTransmission(OLED_ADDR); Wire.write(OLED_COMMAND);
		Wire.write(renderingFrame | (oledY & 0x07));
		uint8_t _oledX = oledX;
#ifdef OLED_SH1106
		_oledX += 2; // SH1106 is a 132x64 controller.  Use middle 128 columns.
#endif
		
		Wire.write(0x10 | ((_oledX & 0xf0) >> 4));
		Wire.write(_oledX & 0x0f);
		Wire.endTransmission();
		
	}
	
	void drawCursor(bool en) {
		Wire.beginTransmission(OLED_ADDR); Wire.write(OLED_DATA);
		Wire.write((en) ? 0xf0 : 0x00);  // horizontal line
		Wire.endTransmission();
	}
	
	void setCursor(uint8_t x, uint8_t y) {
		if (curs) { drawCursor(false); } _setCursor(x * FONT_W, y * FONT_H); if (curs) { drawCursor(true); _setCursor(oledX, oledY); }
	}
	
	void newLine() {
		oledY += FONT_H;
		if (oledY > OLED_PAGES - FONT_H) {
			oledY = OLED_PAGES - FONT_H;
		}
		_setCursor(0, oledY);
	}
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 18/01/2026 - 9M2GRC
// To saved program memory during code compilation, simplified OLED write function 
////////////////////////////////////////////////////////////////////////
#ifndef TRUSDX
	size_t write(byte c) {
		if ((c == '\n') || (oledX > ((uint8_t)128 - FONT_W))) {
			if (wrap)  newLine();
			return 1;
		}
		//if(oledY > OLED_PAGES - FONT_H) return; //needed?
		c = ((c < 9) ? (c + '~') : c) - ' ';

		uint16_t offset = ((uint16_t)c) * FONT_W / (FONT_STRETCHH + 1) * FONT_H;
		uint8_t line = FONT_H;
		do
		{
			if (FONT_STRETCHV) offset = ((uint16_t)c) * FONT_W / (FONT_STRETCHH + 1) * FONT_H / (2 * FONT_STRETCHV);
			Wire.beginTransmission(OLED_ADDR); Wire.write(OLED_DATA);
			for (uint8_t i = 0; i < (FONT_W / (FONT_STRETCHH + 1)); i++) {
				uint8_t b = pgm_read_byte(&(font[offset++]));
				if (FONT_STRETCHV) {
					uint8_t b2 = 0;
					if (line > 1) for (int i = 0; i != 4; i++) b2 |=/* ! */(b & (1 << i)) ? (1 << (i * 2)) | (1 << ((i * 2) + 1)) : 0x00;
					else         for (int i = 0; i != 4; i++) b2 |=/* ! */(b & (1 << (i + 4))) ? (1 << (i * 2)) | (1 << ((i * 2) + 1)) : 0x00;
					Wire.write(b2);
					if (FONT_STRETCHH) Wire.write(b2);
				}
				else { Wire.write(b); if (FONT_STRETCHH) Wire.write(b); }
			}
			Wire.endTransmission();
			if (FONT_H == 1) {
				oledX += FONT_W;
			}
			else {
				if (line > 1) {
					_setCursor(oledX, oledY + 1);
				}
				else {
					_setCursor(oledX + FONT_W, oledY - (FONT_H - 1));
				}
			}
		} while (--line);
		return 1;
	}
	void bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]) {
		uint16_t j = 0;
		for (uint8_t y = y0; y < y1; y++) {
			_setCursor(x0, y);
			Wire.beginTransmission(OLED_ADDR); Wire.write(OLED_DATA);
			for (uint8_t x = x0; x < x1; x++) {
				Wire.write(pgm_read_byte(&bitmap[j++]));
			}
			Wire.endTransmission();
		}
		setCursor(0, 0);
	}
#else
	size_t write(byte c) {
    if ((c == '\n') || (oledX > (128 - FONT_W))) {
        if (wrap) newLine();
        return 1;
    }

    uint8_t idx = 0;
    // Lowercase handled as Uppercase to save space
    if (c >= 'a' && c <= 'z')      c -= 32; 
    
    if (c >= 'A' && c <= 'Z')      idx = c - 'A' + 16;
    else if (c >= '0' && c <= '9') idx = c - '0' + 6;
    else if (c > 0 && c < 9)       idx = c + 41; // Custom Icons
    else {
        switch (c) {
            case '.': idx = 1; break;
            case '+': idx = 2; break;
            case '>': idx = 3; break;
            case ',': idx = 4; break;
            case '-': idx = 5; break;
        }
    }

    uint8_t startX = oledX;
    uint8_t startY = oledY;

    // 1. Draw Character
    _setCursor(startX, startY);
    Wire.beginTransmission(OLED_ADDR);
    Wire.write(OLED_DATA);
    for (uint8_t i = 0; i < 8; i++) {
        Wire.write(pgm_read_byte(&font[(idx << 3) + i]));
    }
    Wire.endTransmission();
	
    // 2. Ensure Line 3 & 4 (Page 2 & 3) are blank
    if (startY < 2) {
        for (uint8_t p = 2; p < 4; p++) {
            _setCursor(startX, p);
            Wire.beginTransmission(OLED_ADDR);
            Wire.write(OLED_DATA);
            for (uint8_t i = 0; i < 8; i++) Wire.write(0x00);
            Wire.endTransmission();
        }
    }
	
    // 3. Return cursor to typing line
    oledX = startX + FONT_W;
    _setCursor(oledX, startY);
    return 1;
	}
#endif // TRUSDX
};
template<class parent>class Display : public parent {  // This class spoofs display contents and cursor state
public:
#ifdef CAT_EXT
	uint8_t x, y;
	bool curs;
	char text[2 * 16 + 1];
	Display() : parent() { clear(); };
	size_t write(uint8_t b) { if ((x < 16) && (y < 2)) { text[y * 16 + x] = ((b < 9) ? "> :*#AB"[b - 1] /*(b + 0x80 - 1)*/ /*(uint8_t[]){ 0xAF, 0x20, 0xB0, 0xB1, 0xB2, 0xA6, 0xE1, 0x20 }[b-1]*/ : b); x++; } return parent::write(b); }
	void setCursor(uint8_t _x, uint8_t _y) { x = _x; y = _y; parent::setCursor(_x, _y); }
	void cursor() { curs = true; parent::cursor(); }
	void noCursor() { curs = false; parent::noCursor(); }
	void clear() { for (uint8_t i = 0; i != 2 * 16; i++) text[i] = ' '; text[2 * 16] = '\0'; x = 0; y = 0; }
#endif //CAT_EXT
};
//#define BLIND 1             // uSDX in head-less operation
#ifdef BLIND
class Blind : public Print {  // This class is a dummy LCD replacement
public:
	size_t write(uint8_t b) {}
	void setCursor(uint8_t _x, uint8_t _y) {}
	void cursor() {}
	void noCursor() {}
	void begin(uint8_t x = 0, uint8_t y = 0) {}
	void noDisplay() {}
	void createChar(uint8_t l, uint8_t glyph[]) {}
};
Display<Blind> lcd;
#else
#ifdef OLED
Display<OLEDDevice> lcd;
#else
Display<LCD> lcd;     // highly-optimized LCD driver, OK for QCX supplied displays
#endif
#endif

#ifdef DEBUG_G8RDI
int fir_value = 0;
#endif

volatile int8_t encoder_val = 0;
volatile int8_t encoder_step = 0;
static uint8_t last_state;
ISR(PCINT2_vect) {  // Interrupt on rotary encoder turn
	switch (last_state = (last_state << 4) | (_digitalRead(ROT_B) << 1) | _digitalRead(ROT_A)) { //transition  (see: https://www.allaboutcircuits.com/projects/how-to-use-a-rotary-encoder-in-a-mcu-based-project/  )
		//#define ENCODER_ENHANCED_RESOLUTION  1
#ifdef ENCODER_ENHANCED_RESOLUTION // Option: enhance encoder from 24 to 96 steps/revolution, see: appendix 1, https://www.sdr-kits.net/documents/PA0KLT_Manual.pdf
	case 0x31: case 0x10: case 0x02: case 0x23: encoder_val++; break;
	case 0x32: case 0x20: case 0x01: case 0x13: encoder_val--; break;
#else
	case 0x23:  encoder_val++; break;
	case 0x32:  encoder_val--; break;
#endif
	}
}
void encoder_setup()
{
	fastpinMode(ROT_A, INPUT_PULLUP);
	fastpinMode(ROT_B, INPUT_PULLUP);
	PCMSK2 |= (1 << PCINT22) | (1 << PCINT23); // interrupt-enable for ROT_A, ROT_B pin changes; see https://github.com/EnviroDIY/Arduino-SDI-12/wiki/2b.-Overview-of-Interrupts
	PCICR |= (1 << PCIE2);
	last_state = (_digitalRead(ROT_B) << 1) | _digitalRead(ROT_A);
	interrupts();
}

// I2C communication starts with a START condition, multiple single byte-transfers (MSB first) followed by an ACK/NACK and stops with a STOP condition;
// during data-transfer SDA may only change when SCL is LOW, during a START/STOP condition SCL is HIGH and SDA goes DOWN for a START and UP for a STOP.
// https://www.ti.com/lit/an/slva704/slva704.pdf
class I2C {
public:
#if(F_MCU > 20900000)
#define I2C_DELAY   6
#else
#define I2C_DELAY   4    // Determines I2C Speed (2=939kb/s (too fast!!); 3=822kb/s; 4=731kb/s; 5=658kb/s; 6=598kb/s). Increase this value when you get I2C tx errors (E05); decrease this value when you get a CPU overload (E01). An increment eats ~3.5% CPU load; minimum value is 3 on my QCX, resulting in 84.5% CPU load
#endif
#define I2C_DDR DDRC     // Pins for the I2C bit banging
#define I2C_PIN PINC
#define I2C_PORT PORTC
#define I2C_SDA (1 << 4) // PC4
#define I2C_SCL (1 << 5) // PC5
#define DELAY(n) for(uint8_t i = 0; i != n; i++) asm("nop");
#define I2C_SDA_GET() I2C_PIN & I2C_SDA
#define I2C_SCL_GET() I2C_PIN & I2C_SCL
#define I2C_SDA_HI() I2C_DDR &= ~I2C_SDA;
#define I2C_SDA_LO() I2C_DDR |=  I2C_SDA;
#define I2C_SCL_HI() I2C_DDR &= ~I2C_SCL; DELAY(I2C_DELAY);
#define I2C_SCL_LO() I2C_DDR |=  I2C_SCL; DELAY(I2C_DELAY);

	I2C() {
		I2C_PORT &= ~(I2C_SDA | I2C_SCL);
		I2C_SCL_HI();
		I2C_SDA_HI();
#ifndef RS_HIGH_ON_IDLE
		suspend();
#endif
	}
	~I2C() {
		I2C_PORT &= ~(I2C_SDA | I2C_SCL);
		I2C_DDR &= ~(I2C_SDA | I2C_SCL);
	}
	inline void start() {
#ifdef RS_HIGH_ON_IDLE
		I2C_SDA_LO();
#else
		resume();  //prepare for I2C
#endif
		I2C_SCL_LO();
		I2C_SDA_HI();
	}
	inline void stop() {
		I2C_SDA_LO();   // ensure SDA is LO so STOP-condition can be initiated by pulling SCL HI (in case of ACK it SDA was already LO, but for a delayed ACK or NACK it is not!)
		I2C_SCL_HI();
		I2C_SDA_HI();
		I2C_DDR &= ~(I2C_SDA | I2C_SCL); // prepare for a start: pull-up both SDA, SCL
#ifndef RS_HIGH_ON_IDLE
		suspend();
#endif
	}
#define SendBit(data, mask) \
    if(data & mask){ \
      I2C_SDA_HI();  \
    } else {         \
      I2C_SDA_LO();  \
    }                \
    I2C_SCL_HI();    \
    I2C_SCL_LO();
	inline void SendByte(uint8_t data) {
		SendBit(data, 1 << 7);
		SendBit(data, 1 << 6);
		SendBit(data, 1 << 5);
		SendBit(data, 1 << 4);
		SendBit(data, 1 << 3);
		SendBit(data, 1 << 2);
		SendBit(data, 1 << 1);
		SendBit(data, 1 << 0);
		I2C_SDA_HI();  // recv ACK
		DELAY(I2C_DELAY);
		I2C_SCL_HI();
		I2C_SCL_LO();
	}
	inline uint8_t RecvBit(uint8_t mask) {
		I2C_SCL_HI();
		uint16_t i = 60000;
		for (; !(I2C_SCL_GET()) && i; i--);  // wait util slave release SCL to HIGH (meaning data valid), or timeout at 3ms
		uint8_t data = I2C_SDA_GET();
		I2C_SCL_LO();
		return (data) ? mask : 0;
	}
	inline uint8_t RecvByte(uint8_t last) {
		uint8_t data = 0;
		data |= RecvBit(1 << 7);
		data |= RecvBit(1 << 6);
		data |= RecvBit(1 << 5);
		data |= RecvBit(1 << 4);
		data |= RecvBit(1 << 3);
		data |= RecvBit(1 << 2);
		data |= RecvBit(1 << 1);
		data |= RecvBit(1 << 0);
		if (last) {
			I2C_SDA_HI();  // NACK
		}
		else {
			I2C_SDA_LO();  // ACK
		}
		DELAY(I2C_DELAY);
		I2C_SCL_HI();
		I2C_SDA_HI();    // restore SDA for read
		I2C_SCL_LO();
		return data;
	}
	inline void resume() {
#ifdef LCD_RS_PORTIO
		I2C_PORT &= ~I2C_SDA; // pin sharing SDA/LCD_RS mitigation
#endif
	}
	inline void suspend() {
		I2C_SDA_LO();         // pin sharing SDA/LCD_RS: pull-down LCD_RS; QCXLiquidCrystal require this for any operation
	}

	void begin() {};
	void beginTransmission(uint8_t addr) { start(); SendByte(addr << 1); };
	bool write(uint8_t byte) { SendByte(byte); return 1; };
	uint8_t endTransmission() { stop(); return 0; };
};

uint8_t log2(uint16_t x) {
	uint8_t y = 0;
	for (; x >>= 1;) y++;
	return y;
}

I2C i2c;
class SI5351 {
public:
	volatile int32_t _fout;
	volatile uint8_t _div;  // note: uint8_t asserts fout > 3.5MHz with R_DIV=1
	volatile uint16_t _msa128min512;
	volatile uint32_t _msb128;
	volatile uint8_t pll_regs[8];

#define BB0(x) ((uint8_t)(x))           // Bash byte x of int32_t
#define BB1(x) ((uint8_t)((x)>>8))
#define BB2(x) ((uint8_t)((x)>>16))

#define FAST __attribute__((optimize("Ofast")))

	volatile uint32_t fxtal = F_XTAL;

#define NEW_TX 1
#ifdef NEW_TX

	inline void FAST freq_calc_fast(int32_t df)  // note: relies on cached variables: _msb128, _msa128min512, _div, _fout, fxtal
	{
#define _MSC  0x10000
		uint32_t msb128 = _msb128 + ((int64_t)(_div * (int32_t)df) * _MSC * 128) / fxtal;

		uint16_t msp1 = _msa128min512 + msb128 / _MSC; // = 128 * _msa + msb128 / _MSC - 512;
		uint16_t msp2 = msb128; // = msb128 % _MSC;  assuming MSC is covering exact uint16_t so the mod operation can dissapear (and the upper BB2 byte) // = msb128 - msb128/_MSC * _MSC;

		pll_regs[4] = BB0(msp1);
		pll_regs[5] = ((_MSC & 0xF0000) >> (16 - 4))/*|BB2(msp2)*/; // top nibble MUST be same as top nibble of _MSC !  assuming that BB2(msp2) is always 0 -> so reg is constant
		pll_regs[6] = BB1(msp2);
		pll_regs[7] = BB0(msp2);
	}

	inline void SendPLLRegisterBulk() {
		i2c.start();
		i2c.SendByte(SI5351_ADDR << 1);
		i2c.SendByte(26 + 0 * 8 + 4);  // Write to PLLA
		i2c.SendByte(pll_regs[4]);
		i2c.SendByte(pll_regs[5]);
		i2c.SendByte(pll_regs[6]);
		i2c.SendByte(pll_regs[7]);
		i2c.stop();
	}
#else  // !NEW_TX
	inline void FAST freq_calc_fast(int32_t df)  // note: relies on cached variables: _msb128, _msa128min512, _div, _fout, fxtal
	{
#define _MSC  0x80000  //0x80000: 98% CPU load   0xFFFFF: 114% CPU load
		uint32_t msb128 = _msb128 + ((int64_t)(_div * (int32_t)df) * _MSC * 128) / fxtal;

		uint32_t msp1 = _msa128min512 + msb128 / _MSC;  // = 128 * _msa + msb128 / _MSC - 512;
		uint32_t msp2 = msb128 % _MSC;  // = msb128 - msb128/_MSC * _MSC;

		pll_regs[3] = BB1(msp1);
		pll_regs[4] = BB0(msp1);
		pll_regs[5] = ((_MSC & 0xF0000) >> (16 - 4)) | BB2(msp2); // top nibble MUST be same as top nibble of _MSC !
		pll_regs[6] = BB1(msp2);
		pll_regs[7] = BB0(msp2);
	}

	inline void SendPLLRegisterBulk() {
		i2c.start();
		i2c.SendByte(SI5351_ADDR << 1);
		i2c.SendByte(26 + 0 * 8 + 3);  // Write to PLLA
		i2c.SendByte(pll_regs[3]);
		i2c.SendByte(pll_regs[4]);
		i2c.SendByte(pll_regs[5]);
		i2c.SendByte(pll_regs[6]);
		i2c.SendByte(pll_regs[7]);
		i2c.stop();
	}
#endif // !NEW_TX

	void SendRegister(uint8_t reg, uint8_t* data, uint8_t n) {
		i2c.start();
		i2c.SendByte(SI5351_ADDR << 1);
		i2c.SendByte(reg);
		while (n--) i2c.SendByte(*data++);
		i2c.stop();
	}
	void SendRegister(uint8_t reg, uint8_t val) { SendRegister(reg, &val, 1); }
	int16_t iqmsa; // to detect a need for a PLL reset
	enum ms_t { PLLA = 0, PLLB = 1, MSNA = -2, MSNB = -1, MS0 = 0, MS1 = 1, MS2 = 2, MS3 = 3, MS4 = 4, MS5 = 5 };

	void ms(int8_t n, uint32_t div_nom, uint32_t div_denom, uint8_t pll = PLLA, uint8_t _int = 0, uint16_t phase = 0, uint8_t rdiv = 0) {
		uint16_t msa; uint32_t msb, msc, msp1, msp2, msp3;
		msa = div_nom / div_denom;     // integer part: msa must be in range 15..90 for PLL, 8+1/1048575..900 for MS
		if (msa == 4) _int = 1;  // To satisfy the MSx_INT=1 requirement of AN619, section 4.1.3 which basically says that for MS divider a value of 4 and integer mode must be used
		msb = (_int) ? 0 : (((uint64_t)(div_nom % div_denom) * _MSC) / div_denom); // fractional part
		msc = (_int) ? 1 : _MSC;
		msp1 = 128 * msa + 128 * msb / msc - 512;
		msp2 = 128 * msb - 128 * msb / msc * msc;
		msp3 = msc;
		uint8_t ms_reg2 = BB2(msp1) | (rdiv << 4) | ((msa == 4) * 0x0C);
		uint8_t ms_regs[8] = { BB1(msp3), BB0(msp3), ms_reg2, BB1(msp1), BB0(msp1), BB2(((msp3 & 0x0F0000) << 4) | msp2), BB1(msp2), BB0(msp2) };

		SendRegister(n * 8 + 42, ms_regs, 8); // Write to MSx
		if (n < 0) {
			SendRegister(n + 16 + 8, 0x80 | (0x40 * _int)); // MSNx PLLn: 0x40=FBA_INT; 0x80=CLKn_PDN
		}
		else {
			SendRegister(n + 16, ((pll) * 0x20) | 0x0C | 3 | (0x40 * _int));  // MSx CLKn: 0x0C=PLLA,0x2C=PLLB local msynth; 3=8mA; 0x40=MSx_INT; 0x80=CLKx_PDN
			SendRegister(n + 165, (!_int) * phase * msa / 90);      // when using: make sure to configure MS in fractional-mode, perform reset afterwards
		}
	}

	void phase(int8_t n, uint32_t div_nom, uint32_t div_denom, uint16_t phase) { SendRegister(n + 165, phase * (div_nom / div_denom) / 90); }  // when using: make sure to configure MS in fractional-mode!, perform reset afterwards

	void reset() { SendRegister(177, 0xA0); } // 0x20 reset PLLA; 0x80 reset PLLB

	void oe(uint8_t mask) { SendRegister(3, ~mask); } // output-enable mask: CLK2=4; CLK1=2; CLK0=1

	void freq(int32_t fout, uint16_t i, uint16_t q) {  // Set a CLK0,1,2 to fout Hz with phase i, q (on PLLA)
		uint8_t rdiv = 0; // CLK pin sees fout/(2^rdiv)
		if (fout > 300000000) { i /= 3; q /= 3; fout /= 3; }  // for higher freqs, use 3rd harmonic
		if (fout < 500000) { rdiv = 7; fout *= 128; } // Divide by 128 for fout 4..500kHz
		uint16_t d; if (fout < 30000000) d = (16 * fxtal) / fout; else d = (32 * fxtal) / fout;  // Integer part  .. maybe 44?
		if (fout < 3500000) d = (7 * fxtal) / fout;  // PLL at 189MHz to cover 160m (freq>1.48MHz) when using 27MHz crystal
		if (fout > 140000000) d = 4; // for f=140..300MHz; AN619; 4.1.3, this implies integer mode
		if (d % 2) d++;  // even numbers preferred for divider (AN619 p.4 and p.6)
		if ((d * (fout - 5000) / fxtal) != (d * (fout + 5000) / fxtal)) d += 2; // Test if multiplier remains same for freq deviation +/- 5kHz, if not use different divider to make same
		uint32_t fvcoa = d * fout;  // Variable PLLA VCO frequency at integer multiple of fout at around 27MHz*16 = 432MHz

		ms(MSNA, fvcoa, fxtal);                   // PLLA in fractional mode
		ms(MS0, fvcoa, fout, PLLA, 0, i, rdiv);  // Multisynth stage with integer divider but in frac mode due to phase setting
		ms(MS1, fvcoa, fout, PLLA, 0, q, rdiv);
#ifdef F_CLK2
		freqb(F_CLK2);
#else
		ms(MS2, fvcoa, fout, PLLA, 0, 0, rdiv);
#endif
		if (iqmsa != (((int8_t)i - (int8_t)q) * ((int16_t)(fvcoa / fout)) / 90)) { iqmsa = ((int8_t)i - (int8_t)q) * ((int16_t)(fvcoa / fout)) / 90; reset(); }
		oe(0b00000011);  // output enable CLK0, CLK1

		_fout = fout;  // cache
		_div = d;
		_msa128min512 = fvcoa / fxtal * 128 - 512;
		_msb128 = ((uint64_t)(fvcoa % fxtal) * _MSC * 128) / fxtal;
	}

	void freqb(uint32_t fout) {  // Set a CLK2 to fout Hz (on PLLB)
		uint16_t d = (16 * fxtal) / fout;
		if (d % 2) d++;  // even numbers preferred for divider (AN619 p.4 and p.6)
		uint32_t fvcoa = d * fout;  // Variable PLLA VCO frequency at integer multiple of fout at around 27MHz*16 = 432MHz

		ms(MSNB, fvcoa, fxtal);
		ms(MS2, fvcoa, fout, PLLB, 0, 0, 0);
	}

	uint8_t RecvRegister(uint8_t reg) {
		i2c.start();  // Data write to set the register address
		i2c.SendByte(SI5351_ADDR << 1);
		i2c.SendByte(reg);
		i2c.stop();
		i2c.start(); // Data read to retrieve the data from the set address
		i2c.SendByte((SI5351_ADDR << 1) | 1);
		uint8_t data = i2c.RecvByte(true);
		i2c.stop();
		return data;
	}
	void powerDown() {
		SendRegister(3, 0b11111111); // Disable all CLK outputs
		SendRegister(24, 0b00000000); // Disable state: LOW state when disabled
		SendRegister(25, 0b00000000); // Disable state: LOW state when disabled
		for (int addr = 16; addr != 24; addr++) SendRegister(addr, 0b10000000);  // Conserve power when output is disabled
		SendRegister(187, 0);        // Disable fanout (power-safe)
		// To initialise things as they should:
		SendRegister(149, 0);        // Disable spread spectrum enable
		SendRegister(183, 0b11010010);  // Internal CL = 10 pF (default)
	}
#define SI_CLK_OE 3

};
static SI5351 si5351;

#ifdef LPF_SWITCHING_DL2MAN_USDX_REV1
class PCA9536 {
public:
#define PCA9536_ADDR  0x41  // PCA9536   https://www.ti.com/lit/ds/symlink/pca9536.pdf
	inline void SendRegister(uint8_t reg, uint8_t val) { i2c.begin(); i2c.beginTransmission(PCA9536_ADDR); i2c.write(reg); i2c.write(val); i2c.endTransmission(); }
	inline void init() { SendRegister(0x03, 0x00); } // configuration cmd: IO0-IO7 as output
	inline void write(uint8_t data) { init(); SendRegister(0x01, data); }  // output port cmd: write bits D7-D0 to IO7-IO0
};
PCA9536 ioext;

void set_latch(uint8_t io) { // reset all latches and set latch k to corresponding GPIO, all relays share a common (ground) GPIO
#define LATCH_TIME  15   // set/reset time latch relay
	for (int i = 0; i != 8; i++) { ioext.write((~(1 << i)) | 0x01); delay(LATCH_TIME); } ioext.write(0x00); // reset all latches
	ioext.write((1 << io) | 0x00); delay(LATCH_TIME); ioext.write(0x00); // set latch wired to io port
}

static uint8_t prev_lpf_io = 0xff;
inline void set_lpf(uint8_t f) {
	uint8_t lpf_io = (f > 8) ? 1 : (f > 4) ? 2 : /*(f <= 4)*/ 3; // cut-off freq in MHz to IO port of LPF relay
	if (prev_lpf_io != lpf_io) { prev_lpf_io = lpf_io; set_latch(lpf_io); };  // set relay
}
#endif  //LPF_SWITCHING_DL2MAN_USDX_REV1

////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 03/01/2026 - 9M2GRC
////////////////////////////////////////////////////////////////////////
#ifdef TRUSDX
class IOExpander16 {
	public:
	#define IOEXP16_ADDR  0x20
	
	inline void SendRegister(uint8_t reg, uint8_t val) { i2c.begin(); i2c.beginTransmission(IOEXP16_ADDR); i2c.write(reg); i2c.write(val); i2c.endTransmission(); }
	inline void init() { write(0U); } //IO0, IO1 as input, IO0 to 0, IO0 as output, IO1 to 0, IO1 as output
	inline void write(uint16_t data) { SendRegister(0x07, 0xff);  SendRegister(0x06, 0xff);/*Common last!*/ SendRegister(0x02, data); SendRegister(0x06, 0x00);/*Common first!*/ SendRegister(0x03, data >> 8); SendRegister(0x07, 0x00); }  // output port cmd: write bits D15-D0 to IO1.7-0.0;
};
IOExpander16 ioext;

enum gpioext_t { IO0_0, IO0_1, IO0_2, IO0_3, IO0_4, IO0_5, IO0_6, IO0_7, IO1_0, IO1_1, IO1_2, IO1_3, IO1_4, IO1_5, IO1_6, IO1_7 };
static uint8_t prev_lpf_io = 0xff; // inits and resets all latches

void set_latch(uint8_t io, uint8_t common_io, bool latch = true) { // reset all latches and set latch k to corresponding GPIO, all relays share a common (ground) GPIO
#define LATCH_TIME  30   // set/reset time latch relay
	if (latch) {
		ioext.write((1U << io) | 0x0000); delay(LATCH_TIME); ioext.write(0x0000); // set latch wired to io port
	}
	else {
		if (io == 0xff) { ioext.init(); for (int io = 0; io != 16; io++) set_latch(io, common_io, latch); } // reset all latches
		else { ioext.write((~(1U << io)) | (1U << common_io)); delay(LATCH_TIME); ioext.write(0x0000); } // reset latch wired to io port
	}
}

inline void set_lpf(uint8_t f) {
	#ifdef LOBAND 
	uint8_t lpf_io = (f > 12) ? IO1_3 : (f > 8) ? IO1_2 : (f > 5) ? IO1_4 : (f > 4) ? IO1_1 : /*(f <= 4)*/ IO1_5; // cut-off freq in MHz to IO port of LPF relay
	if (prev_lpf_io != lpf_io) { set_latch(prev_lpf_io, IO0_0, false); set_latch(lpf_io, IO0_0); prev_lpf_io = lpf_io; };  // set relay
	#endif 
	// CLASSIC Bands - 10/15/20/40/80 meter bands 
	#ifdef CLASSICBAND
	uint8_t lpf_io = (f > 23) ? IO1_3 : (f > 17) ? IO1_2 : (f > 8) ? IO1_4 : (f > 4) ? IO1_1 : /*(f <= 4)*/ IO1_5; // cut-off freq in MHz to IO port of LPF relay
	if (prev_lpf_io != lpf_io) { set_latch(prev_lpf_io, IO0_0, false); set_latch(lpf_io, IO0_0); prev_lpf_io = lpf_io; };  // set relay
	#endif
	// HI Bands - 10/12/15/17/20 meter bands 
	#ifdef HIBAND
	uint8_t lpf_io = (f > 25) ? IO1_3 : (f > 23) ? IO1_2 : (f > 19) ? IO1_4 : (f > 17) ? IO1_1 : /*(f <= 4)*/ IO1_5; // cut-off freq in MHz to IO port of LPF relay
	if (prev_lpf_io != lpf_io) { set_latch(prev_lpf_io, IO0_0, false); set_latch(lpf_io, IO0_0); prev_lpf_io = lpf_io; };  // set relay
	#endif
}	
#else
#if defined(LPF_SWITCHING_DL2MAN_USDX_REV3) || defined(LPF_SWITCHING_DL2MAN_USDX_REV2) || defined(LPF_SWITCHING_DL2MAN_USDX_REV2_BETA)
class IOExpander16 {
public:
#ifdef LPF_SWITCHING_DL2MAN_USDX_REV2_BETA
#define IOEXP16_ADDR  0x74  // PCA9539 with A1..A0 set to 0     https://www.nxp.com/docs/en/data-sheet/PCA9539_PCA9539R.pdf
#endif
#ifdef LPF_SWITCHING_DL2MAN_USDX_REV2
#define IOEXP16_ADDR  0x24  // TCA/PCA9555 with A2=1 A1..A0=0   https://www.ti.com/lit/ds/symlink/tca9555.pdf
#endif
#ifdef LPF_SWITCHING_DL2MAN_USDX_REV3
#define IOEXP16_ADDR  0x20  // TCA/PCA9555 with A2=0 A1..A0=0   https://www.ti.com/lit/ds/symlink/tca9555.pdf
#endif
	inline void SendRegister(uint8_t reg, uint8_t val) { i2c.begin(); i2c.beginTransmission(IOEXP16_ADDR); i2c.write(reg); i2c.write(val); i2c.endTransmission(); }
	inline void init() { write(0U); } //IO0, IO1 as input, IO0 to 0, IO0 as output, IO1 to 0, IO1 as output
	inline void write(uint16_t data) { SendRegister(0x07, 0xff);  SendRegister(0x06, 0xff);/*Common last!*/ SendRegister(0x02, data); SendRegister(0x06, 0x00);/*Common first!*/ SendRegister(0x03, data >> 8); SendRegister(0x07, 0x00); }  // output port cmd: write bits D15-D0 to IO1.7-0.0;
};
IOExpander16 ioext;
enum gpioext_t { IO0_0, IO0_1, IO0_2, IO0_3, IO0_4, IO0_5, IO0_6, IO0_7, IO1_0, IO1_1, IO1_2, IO1_3, IO1_4, IO1_5, IO1_6, IO1_7 };

void set_latch(uint8_t io, uint8_t common_io, bool latch = true) { // reset all latches and set latch k to corresponding GPIO, all relays share a common (ground) GPIO
#define LATCH_TIME  30   // set/reset time latch relay
	if (latch) {
		ioext.write((1U << io) | 0x0000); delay(LATCH_TIME); ioext.write(0x0000); // set latch wired to io port
	}
	else {
		if (io == 0xff) { ioext.init(); for (int io = 0; io != 16; io++) set_latch(io, common_io, latch); } // reset all latches
		else { ioext.write((~(1U << io)) | (1U << common_io)); delay(LATCH_TIME); ioext.write(0x0000); } // reset latch wired to io port
	}
}

static uint8_t prev_lpf_io = 0xff; // inits and resets all latches
inline void set_lpf(uint8_t f) {
#ifdef LPF_SWITCHING_DL2MAN_USDX_REV3
	uint8_t lpf_io = (f > 26) ? IO1_3 : (f > 20) ? IO1_4 : (f > 17) ? IO1_2 : (f > 12) ? IO1_5 : (f > 8) ? IO1_1 : (f > 5) ? IO1_6 : (f > 4) ? IO1_0 : /*(f <= 4)*/ IO1_7; // cut-off freq in MHz to IO port of LPF relay
#ifndef LPF_SWITCHING_DL2MAN_USDX_REV3_NOLATCH
	if (prev_lpf_io != lpf_io) { set_latch(prev_lpf_io, IO0_0, false); set_latch(lpf_io, IO0_0); prev_lpf_io = lpf_io; };  // set relay (latched)
#else
	if (prev_lpf_io != lpf_io) { ioext.write(1U << lpf_io); prev_lpf_io = lpf_io; };  // set relay (non-latched)
#endif //LPF_SWITCHING_DL2MAN_USDX_REV3_NOLATCH
#else //LPF_SWITCHING_DL2MAN_USDX_REV2 LPF_SWITCHING_DL2MAN_USDX_REV2_BETA
	uint8_t lpf_io = (f > 12) ? IO0_3 : (f > 8) ? IO0_5 : (f > 5) ? IO0_7 : (f > 4) ? IO1_1 : /*(f <= 4)*/ IO1_3; // cut-off freq in MHz to IO port of LPF relay
	if (prev_lpf_io != lpf_io) { set_latch(prev_lpf_io, IO0_1, false); set_latch(lpf_io, IO0_1); prev_lpf_io = lpf_io; };  // set relay

#endif
}
#endif  //LPF_SWITCHING_DL2MAN_USDX_REV3 LPF_SWITCHING_DL2MAN_USDX_REV2 REV2_BETA
#endif  // TRUSDX

#ifdef LPF_SWITCHING_WB2CBA_USDX_OCTOBAND
class MCP23008 {
public:
#define MCP23008_ADDR  0x20  // MCP23008 with A1..A0 set to 0   https://ww1.microchip.com/downloads/en/DeviceDoc/21919e.pdf
	inline void SendRegister(uint8_t reg, uint8_t val) { i2c.begin(); i2c.beginTransmission(MCP23008_ADDR); i2c.write(reg); i2c.write(val); i2c.endTransmission(); }
	inline void init() { SendRegister(0x09, 0x00); SendRegister(0x00, 0x00); } //GP0-7 to 0, GP0-7 as output
	inline void write(uint16_t data) { SendRegister(0x09, data); }  // output port cmd: write bits D7-D0 to GP7-GP0
};
MCP23008 ioext;

static uint8_t prev_lpf_io = 0xff; // inits and resets all latches
inline void set_lpf(uint8_t f) {
	uint8_t lpf_io = (f > 26) ? 7 : (f > 20) ? 6 : (f > 17) ? 5 : (f > 12) ? 4 : (f > 8) ? 3 : (f > 6) ? 2 : (f > 4) ? 1 : /*(f <= 4)*/ 0; // cut-off freq in MHz to IO port of LPF relay
	if (prev_lpf_io == 0xff) { ioext.init(); }
	if (prev_lpf_io != lpf_io) { ioext.write(1U << lpf_io); prev_lpf_io = lpf_io; };  // set relay (non-latched)
}
#endif  //LPF_SWITCHING_WB2CBA_USDX_OCTOBAND

#if defined(LPF_SWITCHING_PE1DDA_USDXDUO)
inline void set_lpf(uint8_t f) {
	fastpinMode(PD5, OUTPUT);
	fastdigitalWrite(PD5, (f >= LPF_SWITCHING_PE1DDA_USDXDUO));
}
#endif  //LPF_SWITCHING_PE1DDA_USDXDUO
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 03/01/2026 - 9M2GRC
////////////////////////////////////////////////////////////////////////
#if !defined(LPF_SWITCHING_DL2MAN_USDX_REV1) && !defined(LPF_SWITCHING_DL2MAN_USDX_REV2_BETA) && !defined(LPF_SWITCHING_DL2MAN_USDX_REV2) && !defined(LPF_SWITCHING_DL2MAN_USDX_REV3) && !defined(LPF_SWITCHING_WB2CBA_USDX_OCTOBAND) && !defined(LPF_SWITCHING_PE1DDA_USDXDUO) && !defined(TRUSDX)
inline void set_lpf(uint8_t f) {} // dummy
#endif

#ifdef DEBUG
static uint32_t sr = 0;
static uint32_t cpu_load = 0;
volatile uint16_t param_a = 0;  // registers for debugging, testing and experimental purposes
volatile int16_t param_b = 0;
volatile int16_t param_c = 0;
#endif

enum dsp_cap_t { ANALOG, DSP, SDR };
#ifdef QCX
uint8_t dsp_cap = 0;
uint8_t ssb_cap = 0;
#else
// force SSB and SDR capability
const uint8_t ssb_cap = 1;
const uint8_t dsp_cap = SDR;
#endif

enum mode_t { LSB, USB, CW, FM, AM };
volatile uint8_t mode = USB;
volatile uint16_t numSamples = 0;

volatile uint8_t tx = 0;
volatile uint8_t filt = 0;

inline void _vox(bool trigger)
{
	if (trigger) {
		tx = (tx) ? 254 : 255; // hangtime = 255 / 4402 = 58ms (the time that TX at least stays on when not triggered again). tx == 255 when triggered first, 254 follows for subsequent triggers, until tx is off.
	}
	else {
		if (tx) tx--;
	}
}

#define F_SAMP_TX 4800 //4810 //4805 // 4402 // (Design) ADC sample-rate; is best a multiple of _UA and fits exactly in OCR2A = ((F_CPU / 64) / F_SAMP_TX) - 1 , should not exceed CPU utilization
#if(F_MCU != 20000000)
const int16_t _F_SAMP_TX = (F_MCU * 4800LL / 20000000);  // Actual ADC sample-rate; used for phase calculations
#else
#define _F_SAMP_TX  F_SAMP_TX
#endif
#define _UA  600 //=(_FSAMP_TX)/8 //(_F_SAMP_TX)      //360  // unit angle; integer representation of one full circle turn or 2pi radials or 360 degrees, should be a integer divider of F_SAMP_TX and maximized to have higest precision
#define MAX_DP  ((filt == 0) ? _UA : (filt == 3) ? _UA/4 : _UA/2)     //(_UA/2) // the occupied SSB bandwidth can be further reduced by restricting the maximum phase change (set MAX_DP to _UA/2).
#define CARRIER_COMPLETELY_OFF_ON_LOW  1    // disable oscillator on low amplitudes, to prevent potential unwanted biasing/leakage through PA circuit
#define MULTI_ADC  1  // multiple ADC conversions for more sensitive (+12dB) microphone input
#define QUAD  1       // invert TX signal for phase changes > 180

inline int16_t arctan3(int16_t q, int16_t i)  // error ~ 0.8 degree
{ // source: [1] http://www-labs.iro.umontreal.ca/~mignotte/IFT2425/Documents/EfficientApproximationArctgFunction.pdf
#define _atan2(z)  (_UA/8 + _UA/22 - _UA/22 * z) * z  //derived from (5) [1]   note that atan2 can overflow easily so keep _UA low
	int16_t r;
	if (abs(q) > abs(i))
		r = _UA / 4 - _atan2(abs(i) / abs(q));        // arctan(z) = 90-arctan(1/z)
	else
		r = (i == 0) ? 0 : _atan2(abs(q) / abs(i));   // arctan(z)
	r = (i < 0) ? _UA / 2 - r : r;                  // arctan(-z) = -arctan(z)
	return (q < 0) ? -r : r;                        // arctan(-z) = -arctan(z)
}

#define magn(i, q) (abs(i) > abs(q) ? abs(i) + (abs(q) / 4) : abs(q) + (abs(i) / 4)) // approximation of: magnitude = sqrt(i*i + q*q); error 0.95dB

uint8_t lut[256];
volatile uint8_t amp;
#define MORE_MIC_GAIN   1       // 1.02w adds more microphone gain, improving overall SSB quality (when speaking further away from microphone)
#ifdef MORE_MIC_GAIN
volatile uint8_t vox_thresh = (1 << 2);
#else
volatile uint8_t vox_thresh = (1 << 1); //(1 << 2);
#endif
volatile uint8_t drive = 2;   // hmm.. drive>2 impacts cpu load..why?

static uint8_t cat_enabled = false;  // G8RDI mod - added
static uint8_t quad_enabled = false;  // G8RDI mod - added run time enabling

static uint8_t error_code = 0;      // G8RDI mod - added LCD error code

volatile uint8_t quad = 0;

inline int16_t ssb(int16_t in)
{
	static int16_t dc, z1;

	int16_t i, q;
	uint8_t j;
	static int16_t v[16];
	for (j = 0; j != 15; j++) v[j] = v[j + 1];
#ifdef MORE_MIC_GAIN
#ifdef DIG_MODE
	int16_t ac = in;
	dc = (ac + (7) * dc) / (7 + 1);  // hpf: slow average
	v[15] = (ac - dc) / 2;           // hpf (dc decoupling)  (-6dB gain to compensate for DC-noise)
#else
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 03/02/2026 - 9M2GRC
// (tr)uSDX SSB TX experimentation 
////////////////////////////////////////////////////////////////////////
#ifdef EXPSSB
	int16_t ac = in * 2; //   6dB gain (justified since lpf/hpf is losing -3dB)
	ac = ac + z1;        // lpf
	z1 = (in - (8) * z1) / (8 + 1); // lpf

	// Smmothing clipping limiter 
	if (ac > 250) {
		ac = 250 + (ac - 250) / 2; 
	} else if (ac < -250) {
		ac = -250 - (-250 - ac) / 2;
	}
	dc = (ac + (2) * dc) / (2 + 1);
	v[15] = (ac - dc);
#else 
	int16_t ac = in * 2;             //   6dB gain (justified since lpf/hpf is losing -3dB)
	ac = ac + z1;                    // lpf
	z1 = (in - (2) * z1) / (2 + 1);  // lpf: notch at Fs/2 (alias rejecting)
	dc = (ac + (2) * dc) / (2 + 1);  // hpf: slow average
	v[15] = (ac - dc);               // hpf (dc decoupling)
#endif 
#endif //DIG_MODE
	i = v[7] * 2;  // 6dB gain for i, q  (to prevent quanitization issues in hilbert transformer and phase calculation, corrected for magnitude calc)
	q = ((v[0] - v[14]) * 2 + (v[2] - v[12]) * 8 + (v[4] - v[10]) * 21 + (v[6] - v[8]) * 16) / 64 + (v[6] - v[8]); // Hilbert transform, 40dB side-band rejection in 400..1900Hz (@4kSPS) when used in image-rejection scenario; (Hilbert transform require 5 additional bits)

	uint16_t _amp = magn(i / 2, q / 2);  // -6dB gain (correction)
#else  // !MORE_MIC_GAIN
	dc = (in + dc) / 2;        // average
	int16_t ac = (in - dc);   // DC decoupling
	v[15] = (ac + z1);// / 2;           // low-pass filter with notch at Fs/2
	z1 = ac;

	i = v[7];
	q = ((v[0] - v[14]) * 2 + (v[2] - v[12]) * 8 + (v[4] - v[10]) * 21 + (v[6] - v[8]) * 15) / 128 + (v[6] - v[8]) / 2; // Hilbert transform, 40dB side-band rejection in 400..1900Hz (@4kSPS) when used in image-rejection scenario; (Hilbert transform require 5 additional bits)

	uint16_t _amp = magn(i, q);
#endif  // MORE_MIC_GAIN

#ifdef CARRIER_COMPLETELY_OFF_ON_LOW
	_vox(_amp > vox_thresh);
#else
	if (vox) _vox(_amp > vox_thresh);
#endif

	_amp = _amp << (drive);
	_amp = ((_amp > 255) || (drive == 8)) ? 255 : _amp; // clip or when drive=8 use max output
	amp = (tx) ? lut[_amp] : 0;

	static int16_t prev_phase;
	int16_t phase = arctan3(q, i);

	int16_t dp = phase - prev_phase;  // phase difference and restriction
	prev_phase = phase;

	if (dp < 0)
		dp = dp + _UA; // make negative phase shifts positive: prevents negative frequencies and will reduce spurs on other sideband
#ifdef QUAD       // G8RDI: This worsens TX SSB voice quality, more Dalex sounding.
	if (dp >= (_UA / 2))
	{
		if (quad_enabled)  // G8RDI mod - added
		{
			dp = dp - _UA / 2;
			quad = !quad;
		}
	}
#endif

#ifdef MAX_DP
	if (dp > MAX_DP) { // dp should be less than half unit-angle in order to keep frequencies below F_SAMP_TX/2
		prev_phase = phase - (dp - MAX_DP);  // substract restdp
		dp = MAX_DP;
	}
#endif
	if (mode == USB)
		return dp * (_F_SAMP_TX / _UA); // calculate frequency-difference based on phase-difference
	else
		return dp * (-_F_SAMP_TX / _UA);
}

#define MIC_ATTEN  0  // 0*6dB attenuation (note that the LSB bits are quite noisy)
volatile int8_t mox = 0;
volatile int8_t volume = 12;

// This is the ADC ISR, issued with sample-rate via timer1 compb interrupt.
// It performs in real-time the ADC sampling, calculation of SSB phase-differences, calculation of SI5351 frequency registers and send the registers to SI5351 over I2C.
static int16_t _adc;
void dsp_tx()
{ // jitter dependent things first
#ifdef MULTI_ADC  // SSB with multiple ADC conversions:
	int16_t adc;                         // current ADC sample 10-bits analog input, NOTE: first ADCL, then ADCH
	adc = ADC;
	ADCSRA |= (1 << ADSC);
	si5351.SendPLLRegisterBulk();       // submit frequency registers to SI5351 over 731kbit/s I2C (transfer takes 64/731 = 88us, then PLL-loopfilter probably needs 50us to stabalize)
#ifdef QUAD
	if (quad_enabled)  // G8RDI mod - added
	{
#ifdef TX_CLK0_CLK1
		si5351.SendRegister(16, (quad) ? 0x1f : 0x0f);  // Invert/non-invert CLK0 in case of a huge phase-change
		si5351.SendRegister(17, (quad) ? 0x1f : 0x0f);  // Invert/non-invert CLK1 in case of a huge phase-change
#else
		si5351.SendRegister(18, (quad) ? 0x1f : 0x0f);  // Invert/non-invert CLK2 in case of a huge phase-change
#endif
	}
#endif //QUAD
	OCR1BL = amp;                      // submit amplitude to PWM register (takes about 1/32125 = 31us+/-31us to propagate) -> amplitude-phase-alignment error is about 30-50us
	adc += ADC;
	ADCSRA |= (1 << ADSC);  // causes RFI on QCX-SSB units (not on units with direct biasing); ENABLE this line when using direct biasing!!
	int16_t df = ssb(_adc >> MIC_ATTEN); // convert analog input into phase-shifts (carrier out by periodic frequency shifts)
	adc += ADC;
	ADCSRA |= (1 << ADSC);
	si5351.freq_calc_fast(df);           // calculate SI5351 registers based on frequency shift and carrier frequency
	adc += ADC;
	ADCSRA |= (1 << ADSC);
#define AF_BIAS   32
	_adc = (adc / 4 - (512 - AF_BIAS));        // now make sure that we keep a postive bias offset (to prevent the phase swapping 180 degrees and potentially causing negative feedback (RFI)
#else  // SSB with single ADC conversion:
	ADCSRA |= (1 << ADSC);    // start next ADC conversion (trigger ADC interrupt if ADIE flag is set)
	si5351.SendPLLRegisterBulk();       // submit frequency registers to SI5351 over 731kbit/s I2C (transfer takes 64/731 = 88us, then PLL-loopfilter probably needs 50us to stabalize)
	OCR1BL = amp;                        // submit amplitude to PWM register (takes about 1/32125 = 31us+/-31us to propagate) -> amplitude-phase-alignment error is about 30-50us
	int16_t adc = ADC - 512; // current ADC sample 10-bits analog input, NOTE: first ADCL, then ADCH
	int16_t df = ssb(adc >> MIC_ATTEN);  // convert analog input into phase-shifts (carrier out by periodic frequency shifts)
	si5351.freq_calc_fast(df);           // calculate SI5351 registers based on frequency shift and carrier frequency
#endif

#ifdef CARRIER_COMPLETELY_OFF_ON_LOW
	if (tx == 1) { OCR1BL = 0; si5351.SendRegister(SI_CLK_OE, TX0RX0); }   // disable carrier
	if (tx == 255) { si5351.SendRegister(SI_CLK_OE, TX1RX0); } // enable carrier
#endif

#ifdef MOX_ENABLE
	if (!mox) return;
	OCR1AL = (adc << (mox - 1)) + 128;  // TX audio monitoring
#endif
}

volatile uint16_t acc;
volatile uint32_t cw_offset;
volatile uint8_t tone_vol = 12;
volatile uint8_t cw_tone = 1;
const uint32_t tones[] = { F_MCU * 700ULL / 20000000, F_MCU * 600ULL / 20000000, F_MCU * 700ULL / 20000000 };  // G8RDI todo ULL to divisor?

volatile int8_t p_sin = 0;     // initialized with A*sin(0) = 0
volatile int8_t n_cos = 448 / 4; // initialized with A*cos(t) = A
inline void process_minsky() // Minsky circle sample [source: https://www.cl.cam.ac.uk/~am21/hakmemc.html, ITEM 149]: p_sin+=n_cos*2*PI*f/fs; n_cos-=p_sin*2*PI*f/fs;
{
	int8_t alpha127 = tones[cw_tone]/*cw_offset*/ * 798 / _F_SAMP_TX;  // alpha = f_tone * 2 * pi / fs
	p_sin += alpha127 * n_cos / 127;
	n_cos -= alpha127 * p_sin / 127;
}

// CW Key-click shaping, ramping up/down amplitude with sample-interval of 60us. Tnx: Yves HB9EWY https://groups.io/g/ucx/message/5107
const uint8_t ramp[] PROGMEM = { 255, 254, 252, 249, 245, 239, 233, 226, 217, 208, 198, 187, 176, 164, 152, 139, 127, 115, 102, 90, 78, 67, 56, 46, 37, 28, 21, 15, 9, 5, 2 }; // raised-cosine(i) = 255 * sq(cos(HALF_PI * i/32))

void dummy()
{
}

void dsp_tx_cw()
{ // jitter dependent things first
#ifdef KEY_CLICK
	if (OCR1BL < lut[255]) { //check if already ramped up: ramp up of amplitude 
		for (uint16_t i = 31; i != 0; i--) {   // soft rising slope against key-clicks
			OCR1BL = lut[pgm_read_byte_near(ramp[i])];
			delayMicroseconds(60);
		}
	}
#endif // KEY_CLICK
	OCR1BL = lut[255];

	process_minsky();

#ifdef CW_VOLUME
	OCR1AL = (tone_vol ? (p_sin >> (16 - tone_vol)) : 0) + 128;  // xyzzy G8RDI mod - added for CW tone volume
#else
	OCR1AL = (p_sin >> (16 - volume)) + 128;
#endif
}

void dsp_tx_am()
{ // jitter dependent things first
	ADCSRA |= (1 << ADSC);    // start next ADC conversion (trigger ADC interrupt if ADIE flag is set)
	OCR1BL = amp;                        // submit amplitude to PWM register (actually this is done in advance (about 140us) of phase-change, so that phase-delays in key-shaping circuit filter can settle)
	int16_t adc = ADC - 512; // current ADC sample 10-bits analog input, NOTE: first ADCL, then ADCH
	int16_t in = (adc >> MIC_ATTEN);
	in = in << (drive - 4);
#define AM_BASE 32
	in = max(0, min(255, (in + AM_BASE)));
	amp = in;// lut[in];
}

void dsp_tx_fm()
{ // jitter dependent things first
	ADCSRA |= (1 << ADSC);    // start next ADC conversion (trigger ADC interrupt if ADIE flag is set)
	OCR1BL = lut[255];                   // submit amplitude to PWM register (actually this is done in advance (about 140us) of phase-change, so that phase-delays in key-shaping circuit filter can settle)
	si5351.SendPLLRegisterBulk();       // submit frequency registers to SI5351 over 731kbit/s I2C (transfer takes 64/731 = 88us, then PLL-loopfilter probably needs 50us to stabalize)
	int16_t adc = ADC - 512; // current ADC sample 10-bits analog input, NOTE: first ADCL, then ADCH
	int16_t in = (adc >> MIC_ATTEN);
	in = in << (drive);
	int16_t df = in;
	si5351.freq_calc_fast(df);           // calculate SI5351 registers based on frequency shift and carrier frequency
}

#define EA(y, x, one_over_alpha)  (y) = (y) + ((x) - (y)) / (one_over_alpha); // exponental averaging [Lyons 13.33.1]
#define MLEA(y, x, L, M)  (y)  = (y) + ((((x) - (y)) >> (L)) - (((x) - (y)) >> (M))); // multiplierless exponental averaging [Lyons 13.33.1], with alpha=1/2^L - 1/2^M

#ifdef SWR_METER
volatile uint8_t swrmeter = 1;
#endif

const char m2c[] PROGMEM = "~ ETIANMSURWDKGOHVF*L*PJBXCYZQ**54S3***2**+***J16=/***H*7*G*8*90************?_****\"**.****@***'**-********;!*)*****,****:****";

#ifdef CW_MESSAGE
#define MENU_STR  1

uint8_t delayWithKeySense(uint32_t ms) {
	uint32_t event = millis() + ms;
	for (; millis() < event;) {
		wdt_reset();
		if (inv ^ digitalRead(BUTTONS) || !digitalRead(DAH) || !digitalRead(DIT)) {
			for (; inv ^ digitalRead(BUTTONS);) wdt_reset();  // wait until buttons released  
			return 1;  // stop when button/key pressed
		}
	}
	return 0;
}

// CW Messages can be used edited so allow size.
#ifdef CW_MESSAGE_EXT
char cw_msg[6][CW_MESSAGE_LENGTH] = { CW_MSG1, CW_MSG2, CW_MSG3, CW_MSG4, CW_MSG5, CW_MSG6 };
#else
char cw_msg[1][16] = { CW_STD_MSG };
#endif
uint8_t cw_msg_interval = 5; // number of seconds CW message is repeated
uint32_t cw_msg_event = 0;
uint8_t cw_msg_id = 0; // selected message

int cw_tx(char ch) {    // *** CW Transmit msg ***
	char sym;
	for (uint8_t j = 0; (sym = pgm_read_byte_near(m2c + j)); j++) {  // lookup msg[i] in m2c, skip if not found
		if (sym == ch) {  // found -> transmit CW character j
			wdt_reset();
			uint8_t k = 0x80; for (; !(j & k); k >>= 1); k >>= 1; // shift start of cw code to MSB
			if (k == 0) delay(ditTime * 4); // space -> add word space
			else {
				for (; k; k >>= 1) { // send dit/dah one by one, until everythng is sent
					switch_rxtx(1);  // key-on  tx
					if (delayWithKeySense(ditTime * ((j & k) ? 3 : 1))) { switch_rxtx(0); return 1; } // symbol: dah or dih length
					switch_rxtx(0);  // key-off tx
					if (delayWithKeySense(ditTime)) return 1;   // add symbol space
				}
				if (delayWithKeySense(ditTime * 2)) return 1; // add letter space
			}
			break; // next character
		}
	}
	return 0;
}

int cw_tx(char* msg) {
	for (uint8_t i = 0; msg[i]; i++) {  // loop over message
		lcd.setCursor(0, 0); lcd.print(i); lcd.print("    ");
		if (cw_tx(msg[i]))  // Sent morse char
			return 1;
	}
	return 0;
}
#endif // CW_MESSAGE

volatile uint8_t menumode = 0;  // 0=not in menu, 1=selects menu item, 2=selects parameter value

#ifdef CW_DECODER
volatile uint8_t cwdec = 1;
static int32_t avg = 256;
static uint8_t sym;
static uint32_t amp32 = 0;
volatile uint32_t _amp32 = 0;
static char out[] = "                ";
volatile uint8_t cw_event = false;

void printsym(bool submit = true) {
	if (sym < 128) {
		char ch = pgm_read_byte_near(m2c + sym); if (ch != '*') {
#ifdef CW_INTERMEDIATE
			out[15] = ch; cw_event = true; if (submit) { for (int i = 0; i != 15; i++) { out[i] = out[i + 1]; } out[15] = ' '; }   // update LCD, only shift when submit is true, otherwise update last char only
#else
			for (int i = 0; i != 15; i++) out[i] = out[i + 1]; out[15] = ch; cw_event = true;   // update LCD
#endif
		}
	}
	if (submit) sym = 1;
}

bool realstate = LOW;
bool realstatebefore = LOW;
bool filteredstate = LOW;
bool filteredstatebefore = LOW;
uint8_t nbtime = 16;  // 6 // ms noise blanker
uint32_t starttimehigh;
uint32_t highduration;
uint32_t hightimesavg;
uint32_t lowtimesavg;
uint32_t startttimelow;
uint32_t lowduration;
uint32_t laststarttime = 0;
uint8_t wpm = 25;

inline void cw_decode()
{
	int32_t in = _amp32;
	EA(avg, in, (1 << 8));
	realstate = (in > (avg * 1 / 2));  // threshold

	// here we clean up the state with a noise blanker
	if (realstate != realstatebefore) {
		laststarttime = millis();
	}
	//#define NB_SCALED_TO_WPM    1   // Scales noise-blanker timing the actual CW speed; this should reduce errors from noise at low speeds; this may have side-effect with fast speed changes that fast CW will be filtered out
#ifdef NB_SCALED_TO_WPM
	if ((millis() - laststarttime) > min(1200 / (20 * 2), max(1200 / (40 * 2), hightimesavg / 6))) {
#else
	if ((millis() - laststarttime) > nbtime) {
#endif
		if (realstate != filteredstate) {
			filteredstate = realstate;
		}
	}
	else avg += avg / 100; // keep threshold above noise spikes (increase threshold with 1%)

	dec2();
	realstatebefore = realstate;
	}

#ifdef NEW_CW
void dec2()
{
	// Then we do want to have some durations on high and low
	if (filteredstate != filteredstatebefore) {
		if (menumode == 0) { lcd.noCursor(); lcd.setCursor(15, 1); lcd.print((filteredstate) ? 'R' : ' '); stepsize_showcursor(); }

		if (filteredstate == HIGH) {
			starttimehigh = millis();
			lowduration = (millis() - startttimelow);
		}

		if (filteredstate == LOW) {
			startttimelow = millis();
			highduration = (millis() - starttimehigh);
			if (highduration < (2 * hightimesavg) || hightimesavg == 0) {
				hightimesavg = (highduration + hightimesavg + hightimesavg) / 3;     // now we know avg dit time ( rolling 3 avg)
			}
			if (highduration > (5 * hightimesavg)) {
				hightimesavg = highduration / 3;     // if speed decrease fast ..      
			}
		}
	}

	// now we will check which kind of baud we have - dit or dah, and what kind of pause we do have 1 - 3 or 7 pause, we think that hightimeavg = 1 bit
	if (filteredstate != filteredstatebefore) {
		if (filteredstate == LOW) {  //// we did end a HIGH
#define FAIR_WEIGHTING    1
#ifdef FAIR_WEIGHTING
			if (highduration < (hightimesavg + hightimesavg / 2) && highduration >(hightimesavg * 6 / 10)) { /// 0.6 filter out false dits
#else
			if (highduration < (hightimesavg * 2) && highduration >(hightimesavg * 6 / 10)) { /// 0.6 filter out false dits
#endif
				sym = (sym << 1) | (0);        // insert dit (0)
			}
#ifdef FAIR_WEIGHTING
			if (highduration > (hightimesavg + hightimesavg / 2) && highduration < (hightimesavg * 6)) {
#else
			if (highduration > (hightimesavg * 2) && highduration < (hightimesavg * 6)) {
#endif
				sym = (sym << 1) | (1);        // insert dah (1)
				wpm = (wpm + (1200 / ((highduration) / 3) * 4 / 3)) / 2;
			}
			}

		if (filteredstate == HIGH) {  // we did end a LOW 
			uint16_t lacktime = 10;
			if (wpm > 25)lacktime = 10; // when high speeds we have to have a little more pause before new letter or new word 
			if (wpm > 30)lacktime = 12;
			if (wpm > 35)lacktime = 15;

#ifdef FAIR_WEIGHTING
			if (lowduration > (hightimesavg * (lacktime * 1 / 10)) && lowduration < hightimesavg * (lacktime * 5 / 10)) { // letter space
#else
			if (lowduration > (hightimesavg * (lacktime * 7 / 80)) && lowduration < hightimesavg * (lacktime * 5 / 10)) { // letter space
#endif
				printsym();
			}
			if (lowduration >= hightimesavg * (lacktime * 5 / 10)) { // word space
				printsym();
				printsym();  // print space
			}
			}
		}

	// write if no more letters
	if ((millis() - startttimelow) > (highduration * 6) && (sym > 1)) {
		printsym();
	}

	filteredstatebefore = filteredstate;
			}

#else // OLD_CW

void dec2()
{
	if (filteredstate != filteredstatebefore) { // then we do want to have some durations on high and low
		if (menumode == 0) { lcd.noCursor(); lcd.setCursor(15, 1); lcd.print((filteredstate) ? 'R' : ' '); stepsize_showcursor(); }

		if (filteredstate == HIGH) {
			starttimehigh = millis();
			lowduration = (millis() - startttimelow);

			if ((sym > 1) && lowduration > (hightimesavg * 2)) { // letter space
				printsym();
				wpm = (1200 / hightimesavg * 4 / 3);
			}
			if (lowduration >= hightimesavg * (5)) { sym = 1; printsym(); } // (print additional space) word space
		}

		if (filteredstate == LOW) {
			startttimelow = millis();
			highduration = (millis() - starttimehigh);
			if (highduration < (2 * hightimesavg) || hightimesavg == 0) {
				hightimesavg = (highduration + hightimesavg + hightimesavg) / 3;     // now we know avg dit time (rolling 3 avg)
			}
			if (highduration > (5 * hightimesavg)) {
				hightimesavg = highduration / 3;     // if speed decrease fast ..      
			}
			if (highduration > (hightimesavg / 2)) {
				sym = (sym << 1) | (highduration > (hightimesavg * 2));       // dit (0) or dash (1)
#if defined(CW_INTERMEDIATE) && !defined(OLED) && !defined(LCD_I2C) && (F_MCU >= 20000000)
				printsym(false);
#endif
			}
		}
	}

	if (((millis() - startttimelow) > hightimesavg * (6)) && (sym > 1)) {
		printsym();  // write if no more letters
	}

	filteredstatebefore = filteredstate;
}
#endif //OLD_CW
#endif  //CW_DECODER

#define F_SAMP_PWM (78125/1)
#define F_SAMP_RX 62500
#define F_ADC_CONV (192307/2)  //was 192307/1, but as noted this produces clicks in audio stream. Slower ADC clock cures this (but is a problem for VOX when sampling mic-input simulatanously).

#ifdef FAST_AGC
volatile uint8_t agc = 2;
#else
volatile uint8_t agc = 1;
#endif
volatile uint8_t nr = 2;    // G8RDI mod
volatile uint8_t att = 0;
volatile uint8_t att2 = 2;  // Minimum att2 increased, to prevent numeric overflow on strong signals
volatile uint8_t _init = 0;

static int16_t gain = 1024;
inline int16_t process_agc_fast(int16_t in)
{
	int16_t out = (gain >= 1024) ? (gain >> 10) * in : in;
	int16_t accum = (1 - abs(out >> 10));
	if ((INT16_MAX - gain) > accum) gain = gain + accum;
	if (gain < 1) gain = 1;
	return out;
}

static int16_t centiGain = 128;
#define DECAY_FACTOR 400      // AGC decay occurs <DECAY_FACTOR> slower than attack.
static uint16_t decayCount = DECAY_FACTOR;
#define HI(x)  ((x) >> 8)
#define LO(x)  ((x) & 0xFF)

inline int16_t process_agc(int16_t in)
{
	static bool small = true;
	int16_t out;

	if (centiGain >= 128)
		out = (centiGain >> 5) * in;         // net gain >= 1
	else
		out = (centiGain >> 2) * (in >> 3);  // net gain < 1
	out >>= 2;

	if (HI(abs(out)) > HI(1536)) {
		centiGain -= (centiGain >> 4);       // Fast attack time when big signal encountered (relies on CentiGain >= 16)
	}
	else {
		if (HI(abs(out)) > HI(1024))
			small = false;
		if (--decayCount == 0) {               // But slow ramp up of gain when signal disappears
			if (small) {                         // 400 samples below lower threshold - increase gain
				if (centiGain < (INT16_MAX - (INT16_MAX >> 4)))
					centiGain += (centiGain >> 4);
				else
					centiGain = INT16_MAX;
			}
			decayCount = DECAY_FACTOR;
			small = true;
		}
	}
	return out;
}

inline int16_t process_nr_old(int16_t ac)
{
	ac = ac >> (6 - abs(ac));  // non-linear below amp of 6; to reduce noise (switchoff agc and tune-up volume until noise dissapears, todo:extra volume control needed)
	ac = ac << 3;
	return ac;
}

inline int16_t process_nr_old2(int16_t ac)
{
	static int16_t ea1;
	ea1 = EA(ea1, ac, 64); // alpha=1/64=0.0156
	return ea1;
}

#ifdef NR_FIR
#define MAXfilterWindowSize 13  // window size must be odd, 7(nr=1), 9(2), 11(3), 13(4), 21(5)
static int16_t filterWindowSize;
static int16_t DataList[MAXfilterWindowSize];
static int16_t CoefList[MAXfilterWindowSize];

//byte filterWindowSize;  // number of samples window 
static byte OldestDataPoint = 0;  // oldest data point location 

void FirFilterSetup(byte filterSize, int cutOffFreq, int sampleFreq) // initialize filter
{
#ifdef DEBUG_G8RDI
	fir_value = filterSize;
#endif

	// Fixing best levels here to be used to Full b/w and Att2 = 2
	if (nr == 4)
	{
		filterSize = 7;
		cutOffFreq = 2700;
	}
	else
		if (nr == 5)
		{
			filterSize = 7;
			cutOffFreq = 2200;
		}
		else
			if (nr == 6)
			{
				filterSize = 7;
				cutOffFreq = 1800;
			}
			else
				if (nr == 7)
				{
					filterSize = 7;
					cutOffFreq = 1600;
				}
				else
					if (nr > 7)
					{
						filterSize = 7;
						cutOffFreq = 500;
					}

	if (!(B00000001 & filterSize))
		filterSize++; // window size must be odd
	if (filterSize > MAXfilterWindowSize)
		filterWindowSize = MAXfilterWindowSize;
	else
		filterWindowSize = filterSize;

	if (cutOffFreq > (sampleFreq / 2) - 1)
		cutOffFreq = (sampleFreq / 2) - 1;  // cut off must be less than half sampling freq

	float normalizedCutoff = float(cutOffFreq) / float(sampleFreq);

	int filterOrderHalf = (filterWindowSize - 1) / 2;
	for (int n = 0; n < filterWindowSize; n++) // Generate weight values
	{
		float stepsFromCenter;
		float weight;

		stepsFromCenter = float(n - filterOrderHalf);

		if (n != filterOrderHalf)  // either side of center
		{
			weight = sin(6.2831853f * normalizedCutoff * stepsFromCenter) / (3.14159265f * stepsFromCenter);
		}
		else            // center point
		{
			weight = 2.0f * normalizedCutoff;
		}

		weight *= 0.54f - 0.46f * cos(6.2832f * float(n) / float(filterWindowSize - 1)); // apply Hamming window to reduce ringing 
		CoefList[n] = int(weight * 32767.0f); // offset by 15bits, convert to INT
	}
}

#ifdef AM_MOD_MAGN_SQRT
#define MultiS16X16to32(longRes, intIn1, intIn2) \
asm volatile ( \
"clr r26 \n\t" \
"mul %A1, %A2 \n\t" \
"movw %A0, r0 \n\t" \
"muls %B1, %B2 \n\t" \
"movw %C0, r0 \n\t" \
"mulsu %B2, %A1 \n\t" \
"sbc %D0, r26 \n\t" \
"add %B0, r0 \n\t" \
"adc %C0, r1 \n\t" \
"adc %D0, r26 \n\t" \
"mulsu %B1, %A2 \n\t" \
"sbc %D0, r26 \n\t" \
"add %B0, r0 \n\t" \
"adc %C0, r1 \n\t" \
"adc %D0, r26 \n\t" \
"clr r1 \n\t" \
: \
"=&r" (longRes) \
: \
"a" (intIn1), \
"a" (intIn2) \
: \
"r26" \
)

int32_t inline amul(int16_t a, int16_t b) {
	int32_t r;
	MultiS16X16to32(r, a, b);
	return r;
}
#endif

union TypeConverter
{
	long L;
	int16_t  I[2];
} result;

inline int16_t process_nr(int16_t in)   // G8RDI mod - added DSP FIR filte after NR = 3
{
	if (nr < 3)     // NR 1-2 use 1.02x 2-sample method
	{
		static int16_t ea1;
		ea1 = EA(ea1, in, 1 << (nr - 1));
		return ea1;
	}

	DataList[OldestDataPoint] = in; // replace oldest data in list
	result.L = amul(DataList[OldestDataPoint], CoefList[0]); // accumulate data in a Long to eliminate rounding errors

	for (int i = 1; i < filterWindowSize; i++) // iterate through the rest of the data (stopping one short of start point)
	{
		OldestDataPoint++;  // increment and wrap pointer
		if (OldestDataPoint >= filterWindowSize)
			OldestDataPoint = 0;

		result.L += amul(DataList[OldestDataPoint], CoefList[i]); // ASM 32bit = 16bit x 16bit
	}

	result.L = result.L << 1; // bitshift to make up for non 16bit coefficient offset

	return result.I[1]; // the H16 part of the Long is the result
}
#else
inline int16_t process_nr(int16_t in)
{
	static int16_t ea1;
	ea1 = EA(ea1, in, 1 << (nr - 1));
	return ea1;
}
#endif

#define N_FILT 7
uint8_t prev_filt[] = { 0 , 4 }; // default filter for modes resp. CW, SSB

inline int16_t filt_var(int16_t za0)  //filters build with www.micromodeler.com
{
	static int16_t za1, za2;
	static int16_t zb0, zb1, zb2;
	static int16_t zc0, zc1, zc2;

	if (filt < 4)
	{  // for SSB filters
		static int16_t zz1, zz2;
		zz2 = zz1;
		zz1 = za0;
		za0 = (30 * (za0 - zz2) + 25 * zz1) / 32;                                  //300-Hz

		// 4th Order (SR=8kHz) IIR in Direct Form I, 8x8:16
		switch (filt) {
		case 1: zb0 = (za0 + 2 * za1 + za2) / 2 - (13 * zb1 + 11 * zb2) / 16; break;   // 0-2900Hz filter, first biquad section
		case 2: zb0 = (za0 + 2 * za1 + za2) / 2 - (2 * zb1 + 8 * zb2) / 16; break;     // 0-2400Hz filter, first biquad section
		case 3: zb0 = (za0 + 2 * za1 + za2) / 2 - (0 * zb1 + 4 * zb2) / 16; break;     //0-1800Hz  elliptic
		}

		switch (filt) {
		case 1: zc0 = (zb0 + 2 * zb1 + zb2) / 2 - (18 * zc1 + 11 * zc2) / 16; break;     // 0-2900Hz filter, second biquad section
		case 2: zc0 = (zb0 + 2 * zb1 + zb2) / 4 - (4 * zc1 + 8 * zc2) / 16; break;       // 0-2400Hz filter, second biquad section
		case 3: zc0 = (zb0 + 2 * zb1 + zb2) / 4 - (0 * zc1 + 4 * zc2) / 16; break;       //0-1800Hz  elliptic
		}

		zc2 = zc1;
		zc1 = zc0;

		zb2 = zb1;
		zb1 = zb0;

		za2 = za1;
		za1 = za0;

		return zc0;
	}
	else { // for CW filters
		//   (2nd Order (SR=4465Hz) IIR in Direct Form I, 8x8:16), adding 64x front-gain (to deal with later division)
#ifdef FILTER_700HZ
		if (cw_tone == 0) {
			switch (filt) {
			case 4: zb0 = (za0 + 2 * za1 + za2) / 2 + (41L * zb1 - 23L * zb2) / 32; break;   //500-1000Hz       // FILTER_700HZ for 700 Hz CW tone
			case 5: zb0 = 5 * (za0 - 2 * za1 + za2) + (105L * zb1 - 58L * zb2) / 64; break;   //650-840Hz
			case 6: zb0 = 3 * (za0 - 2 * za1 + za2) + (108L * zb1 - 61L * zb2) / 64; break;   //650-750Hz
			case 7: zb0 = (2 * za0 - 3 * za1 + 2 * za2) + (111L * zb1 - 62L * zb2) / 64; break; //630-680Hz       
			}
			switch (filt) {
			case 4: zc0 = (zb0 - 2 * zb1 + zb2) / 4 + (105L * zc1 - 52L * zc2) / 64; break;      //500-1000Hz
			case 5: zc0 = ((zb0 + 2 * zb1 + zb2) + 97L * zc1 - 57L * zc2) / 64; break;      //650-840Hz
			case 6: zc0 = ((zb0 + zb1 + zb2) + 104L * zc1 - 60L * zc2) / 64; break;       //650-750Hz
			case 7: zc0 = ((zb1)+109L * zc1 - 62L * zc2) / 64; break;               //630-680Hz
			}
		}
		if (cw_tone == 1)
#endif
		{
			switch (filt) {
			case 4: zb0 = (0 * za0 + 1 * za1 + 0 * za2) + (114L * zb1 - 57L * zb2) / 64; break; //600Hz+-250Hz
			case 5: zb0 = (0 * za0 + 1 * za1 + 0 * za2) + (113L * zb1 - 60L * zb2) / 64; break; //600Hz+-100Hz
			case 6: zb0 = (0 * za0 + 1 * za1 + 0 * za2) + (110L * zb1 - 62L * zb2) / 64; break; //600Hz+-50Hz
			case 7: zb0 = (0 * za0 + 1 * za1 + 0 * za2) + (110L * zb1 - 61L * zb2) / 64; break; //600Hz+-18Hz
			}

			switch (filt) {
			case 4: zc0 = (zb0 - 2 * zb1 + zb2) / 1 + (95L * zc1 - 52L * zc2) / 64; break; //600Hz+-250Hz
			case 5: zc0 = (zb0 - 2 * zb1 + zb2) / 4 + (106L * zc1 - 59L * zc2) / 64; break; //600Hz+-100Hz
			case 6: zc0 = (zb0 - 2 * zb1 + zb2) / 16 + (113L * zc1 - 62L * zc2) / 64; break; //600Hz+-50Hz
			case 7: zc0 = (zb0 - 2 * zb1 + zb2) / 32 + (112L * zc1 - 62L * zc2) / 64; break; //600Hz+-18Hz
			}
		}
		zc2 = zc1;
		zc1 = zc0;

		zb2 = zb1;
		zb1 = zb0;

		za2 = za1;
		za1 = za0;

		return zc0 / 8; // compensate the front-end gain
	}
}

#define __UA   256
inline int16_t _arctan3(int16_t q, int16_t i)
{
#define __atan2(z)  (__UA/8  + __UA/22) * z  // very much of a simplification...not accurate at all, but fast
	int16_t r;
	if (abs(q) > abs(i))
		r = __UA / 4 - __atan2(abs(i) / abs(q));        // arctan(z) = 90-arctan(1/z)
	else
		r = (i == 0) ? 0 : __atan2(abs(q) / abs(i));   // arctan(z)
	r = (i < 0) ? __UA / 2 - r : r;                  // arctan(-z) = -arctan(z)
	return (q < 0) ? -r : r;                        // arctan(-z) = -arctan(z)
}

static uint32_t absavg256 = 0;
volatile uint32_t _absavg256 = 0;
volatile int16_t i, q;
int16_t v[14];  // Process Q (down-sampled) samples
int16_t vi[7];  // G8RDI mod, global & renamed 2nd v array to avoid confusions

inline int16_t slow_dsp(int16_t i_ac2, int16_t q_ac2)
{
	// Post processing I and Q (down-sampled) results here - G8DI mod
	int16_t ac, qh, acm;

	q_ac2 >>= att2;  // digital gain control

	if (mode != AM && mode != FM)      // G8RDI AM/FM modd
	{   // !!!! NOTE: Keep this block separated as we have two different static v[] arrays here!!

		qh = ((v[0] - q_ac2) + (v[2] - v[12]) * 4) / 64 + ((v[4] - v[10]) + (v[6] - v[8])) / 8 + ((v[4] - v[10]) * 5 - (v[6] - v[8])) / 128 + (v[6] - v[8]) / 2;

		// Shuffle Q sample along local v[13] -> v[0]:
		v[0] = v[1]; v[1] = v[2]; v[2] = v[3]; v[3] = v[4]; v[4] = v[5]; v[5] = v[6]; v[6] = v[7]; v[7] = v[8]; v[8] = v[9]; v[9] = v[10]; v[10] = v[11]; v[11] = v[12]; v[12] = v[13]; v[13] = q_ac2;
	}   // Do not remove, separate block for separate v[]!!!!

	i_ac2 >>= att2;  // digital gain control

	i = i_ac2; q = q_ac2;   // tbd: this can be more efficient (G8RDI, for what, for AM and FM modes that don't use Hilbert?)

	// Shuffle I sample from below v[6] to v[0]
	int16_t id = vi[0]; vi[0] = vi[1]; vi[1] = vi[2]; vi[2] = vi[3]; vi[3] = vi[4]; vi[4] = vi[5]; vi[5] = vi[6]; vi[6] = i_ac2;  // Delay to match Hilbert transform on Q branch
	// G8RDI mod - changed name from "i" to "id" as a global virtual with same name exists, likely to confuse!

	if (mode == AM)
	{
		acm = -i - q;  // S-Meter

#ifndef AM_MOD_MAGN_SQRT
		ac = magn(i, q);  // AM = SQRT (I * I + Q * Q)
#else
		int32_t iR, qR;
		int16_t i2 = i, q2 = q;
		MultiS16X16to32(iR, i, i2);
		MultiS16X16to32(qR, q, q2);
		ac = (int16_t)sqrt(iR + qR);
#endif
		static int16_t as_last;   // GW8RDI mod - replaced LP filter: DC removal done in sdr_rx()
		int16_t as = ac + (int16_t)((float)as_last * 0.9999f); // Reduce from 0.9999f for less bass response
		ac = as - as_last;
		as_last = as;
	}
	else
		if (mode == FM)
		{
			acm = -i - q;  // S-Meter
#ifdef FM_ARCTAN  // G8RDI mod - enabled FM differentiator
			int16_t z0 = _arctan3(q, i);
			static int16_t z1 = z0; // G8RDI mod - initialised static
			ac = z0 - z1; // Differentiator
			z1 = z0;
#else
			static int16_t zi = i; // G8RDI mod - Note, zi used without being initialised
			ac = ((ac + i) * zi);  // -qh = ac + i
			zi = i;
#endif
		}
		else
		{  // USB, LSB, CW
			acm = -id - qh;  // SSB & CW: inverting I and Q helps dampening a feedback-loop between PWM out and ADC inputs
			ac = acm;
		}

	static uint8_t absavg256cnt;
	if (!(absavg256cnt--)) { _absavg256 = absavg256; absavg256 = 0; }   // Set S-Meter level
	else absavg256 += abs(acm); // G8RDI mod - acm

#ifdef FAST_AGC
	if (agc == 2) {
		ac = process_agc(ac);
		ac = ac >> (16 - volume);
	}
	else if (agc == 1) {
		ac = process_agc_fast(ac);
		ac = ac >> (16 - volume);
#else
	if (agc == 1) {
		ac = process_agc_fast(ac);
		ac = ac >> (16 - volume);
#endif //!FAST_AGC
	}
	else {
		if (volume <= 13)    // if no AGC allow volume control to boost weak signals
			ac = ac >> (13 - volume);
		else
			ac = ac << (volume - 13);
	}
	if (nr) ac = process_nr(ac);

	if (filt) ac = filt_var(ac);

#ifdef CW_DECODER
	if (!(absavg256cnt % 64)) { _amp32 = amp32; amp32 = 0; }
	else amp32 += abs(acm);     // G8RDI mod - acm
#endif  //CW_DECODER

	ac = min(max(ac, -512), 511);	// Limit to -511 to +511

#ifdef QCX
	if (!dsp_cap)
		return 0;  // in QCX-SSB mode (no DSP), slow_dsp() should return 0 (in order to prevent upsampling filter to generate audio)
#endif
	return ac;
}

volatile uint8_t cat_streaming = 0;
volatile uint8_t _cat_streaming = 0;

typedef void(*func_t)(void);
volatile func_t func_ptr;
#undef  R  // Decimating 2nd Order CIC filter
#define R 4  // Rate change from 62500/2 kSPS to 7812.5SPS, providing 12dB gain

#ifndef SIMPLE_RX
volatile uint8_t admux[3];
volatile int16_t ocomb, qh;
volatile uint8_t rx_state = 0;

#pragma GCC push_options
#pragma GCC optimize ("Ofast")  // compiler-optimization for speed

#define NEW_RX  1   // Faster (3rd-order) CIC stage, with simultanuous processing capability
#ifdef NEW_RX
#define AF_OUT  1   // Enables audio output stage (can be disabled in conjunction with CAT_STREAMING to save memory)

static uint8_t tc = 0;

void process(int16_t i_ac2, int16_t q_ac2)
{
	static int16_t ac3;
#ifdef CAT_STREAMING
	if (cat_enabled && cat_streaming) { uint8_t out = ac3 + 128; if (out == ';') out++; Serial.write(out); }
#endif // CAT_STREAMING

#ifdef AF_OUT
	static int16_t ozd1, ozd2;  // Audio output stage
	if (_init)
	{
		ac3 = 0; ozd1 = 0; ozd2 = 0; _init = 0;       // G8RDI mod - todo move to Setup() // hack: on first sample init accumlators of further stages (to prevent instability)
	}

	int16_t od1 = ac3 - ozd1; // Comb section
	ocomb = od1 - ozd2;
#endif //AF_OUT

#define OUTLET  1
#ifdef OUTLET
	if (tc++ == 0)   // prevent recursion
#endif
		interrupts();  // hack, since slow_dsp process exceeds rx sample-time, allow subsequent 7 interrupts for further rx sampling while processing, prevent nested interrupts with tc

#ifdef AF_OUT
	ozd2 = od1;
	ozd1 = ac3;
#endif  //AF_OUT

	ac3 = slow_dsp(i_ac2, q_ac2);  // G8RDI mod - SSB & CW: inverting I and Q helps dampening a feedback-loop between PWM out and ADC inputs

#ifdef OUTLET
	tc--;
#endif
}

static int16_t i_s0za1, i_s0zb0, i_s0zb1, i_s1za1, i_s1zb0, i_s1zb1;
static int16_t q_s0za1, q_s0zb0, q_s0zb1, q_s1za1, q_s1zb0, q_s1zb1, q_ac2;

// G8RDI mod - added pre-definitions for Visual Studio errors
void sdr_rx_03(); void sdr_rx_07();

#define M_SR  1  // CIC N=3
void sdr_rx_00() { int16_t ac = sdr_rx_common_i(); func_ptr = sdr_rx_01;  int16_t i_s1za0 = (ac + (i_s0za1 + i_s0zb0) * 3 + i_s0zb1) >> M_SR; i_s0za1 = ac; int16_t ac2 = (i_s1za0 + (i_s1za1 + i_s1zb0) * 3 + i_s1zb1); i_s1za1 = i_s1za0; process(ac2, q_ac2); }
void sdr_rx_02() { int16_t ac = sdr_rx_common_i(); func_ptr = sdr_rx_03;  i_s0zb1 = i_s0zb0; i_s0zb0 = ac; }
void sdr_rx_04() { int16_t ac = sdr_rx_common_i(); func_ptr = sdr_rx_05;  i_s1zb1 = i_s1zb0; i_s1zb0 = (ac + (i_s0za1 + i_s0zb0) * 3 + i_s0zb1) >> M_SR; i_s0za1 = ac; }
void sdr_rx_06() { int16_t ac = sdr_rx_common_i(); func_ptr = sdr_rx_07;  i_s0zb1 = i_s0zb0; i_s0zb0 = ac; }

void sdr_rx_01() { int16_t ac = sdr_rx_common_q(); func_ptr = sdr_rx_02;  q_s0zb1 = q_s0zb0; q_s0zb0 = ac; }
void sdr_rx_03() { int16_t ac = sdr_rx_common_q(); func_ptr = sdr_rx_04;  q_s1zb1 = q_s1zb0; q_s1zb0 = (ac + (q_s0za1 + q_s0zb0) * 3 + q_s0zb1) >> M_SR; q_s0za1 = ac; }
void sdr_rx_05() { int16_t ac = sdr_rx_common_q(); func_ptr = sdr_rx_06;  q_s0zb1 = q_s0zb0; q_s0zb0 = ac; }
void sdr_rx_07() { int16_t ac = sdr_rx_common_q(); func_ptr = sdr_rx_00;  int16_t q_s1za0 = (ac + (q_s0za1 + q_s0zb0) * 3 + q_s0zb1) >> M_SR; q_s0za1 = ac; q_ac2 = (q_s1za0 + (q_s1za1 + q_s1zb0) * 3 + q_s1zb1); q_s1za1 = q_s1za0; }

static int16_t ozi1, ozi2;

inline int16_t sdr_rx_common_q() {
	ADMUX = admux[0]; ADCSRA |= (1 << ADSC); int16_t ac = ADC - 511;
	return ac;
}

inline int16_t sdr_rx_common_i()    // Get RX AC samples
{
	ADMUX = admux[1]; ADCSRA |= (1 << ADSC);

	int16_t adc = ADC - 511;	// DC removal based on ADC input bias voltage

	static int16_t prev_adc;
	int16_t ac = (prev_adc + adc) >> 1;	// GW8RDI Shift faster for div
	prev_adc = adc;

#ifdef AF_OUT
	if (_init) { ocomb = 0; ozi1 = 0; ozi2 = 0; }
	ozi2 = ozi1 + ozi2;          // Integrator section
	ozi1 = ocomb + ozi1;
	OCR1AL = min(max((ozi2 >> 5) + 128, 0), 255);   // Output to audio PWM port
#endif // AF_OUT
	return ac;
}
#endif //NEW_RX

#endif  //!SIMPLE_RX

ISR(TIMER2_COMPA_vect)  // Timer2 COMPA interrupt
{
	func_ptr();
#ifdef DEBUG
	numSamples++;
#endif
}

#pragma GCC pop_options  // end of DSP section

void adc_start(uint8_t adcpin, bool ref1v1, uint32_t fs)
{
	DIDR0 |= (1 << adcpin); // disable digital input 
	ADCSRA = 0;             // clear ADCSRA register
	ADCSRB = 0;             // clear ADCSRB register
	ADMUX = 0;              // clear ADMUX register
	ADMUX |= (adcpin & 0x0f);    // set analog input pin
	ADMUX |= ((ref1v1) ? (1 << REFS1) : 0) | (1 << REFS0);  // If reflvl == true, set AREF=1.1V (Internal ref); otherwise AREF=AVCC=(5V)
	ADCSRA |= ((uint8_t)log2((uint8_t)(F_CPU / 13 / fs))) & 0x07;  // ADC Prescaler (for normal conversions non-auto-triggered): ADPS = log2(F_CPU / 13 / Fs) - 1; ADSP=0..7 resulting in resp. conversion rate of 1536, 768, 384, 192, 96, 48, 24, 12 kHz
	ADCSRA |= (1 << ADEN);  // enable ADC
#ifdef ADC_NR
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
#endif
}

void adc_stop()
{
	ADCSRA &= ~(1 << ADIE);  // disable interrupts when measurement complete
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescaler for 9.6kHz
#ifdef ADC_NR
	sleep_disable();
#endif
	ADMUX = (1 << REFS0);  // restore reference voltage AREF (5V)
}

void timer1_start(uint32_t fs)
{  // Timer 1: OC1A and OC1B in PWM mode
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); // Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM (non-inverting mode)
	TCCR1B |= (1 << CS10) | (1 << WGM13) | (1 << WGM12); // Mode 14 - Fast PWM;  CS10: clkI/O/1 (No prescaling)
	ICR1H = 0x00;
	ICR1L = min(255, F_CPU / fs);  // PWM value range (fs>78431):  Fpwm = F_CPU / [Prescaler * (1 + TOP)]
	OCR1AH = 0x00;
	OCR1AL = 0x00;  // OC1A (SIDETONE) PWM duty-cycle (span defined by ICR).
	OCR1BH = 0x00;
	OCR1BL = 0x00;  // OC1B (KEY_OUT) PWM duty-cycle (span defined by ICR).
}

void timer1_stop()
{
	OCR1AL = 0x00;
	OCR1BL = 0x00;
}

void timer2_start(uint32_t fs)
{  // Timer 2: interrupt mode
	ASSR &= ~(1 << AS2);  // Timer 2 clocked from CLK I/O (like Timer 0 and 1)
	TCCR2A = 0;
	TCCR2B = 0;
	TCNT2 = 0;
	TCCR2A |= (1 << WGM21); // WGM21: Mode 2 - CTC (Clear Timer on Compare Match)
	TCCR2B |= (1 << CS22);  // Set C22 bits for 64 prescaler
	TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt TIMER2_COMPA_vect
	uint8_t ocr = ((F_CPU / 64) / fs) - 1;   // OCRn = (F_CPU / pre-scaler / fs) - 1;
	OCR2A = ocr;
}

void timer2_stop()
{ // Stop Timer 2 interrupt
	TIMSK2 &= ~(1 << OCIE2A);  // disable timer compare interrupt
	delay(1);  // wait until potential in-flight interrupts are finished
}

void inline lcd_blanks() { lcd.print(F("        ")); }

#define N_FONTS  8
const byte fonts[N_FONTS][8] PROGMEM = {
{ 0b01000,  // 1; logo
  0b00100,
  0b01010,
  0b00101,
  0b01010,
  0b00100,
  0b01000,
  0b00000 },
{ 0b00000,  // 2; s-meter, 0 bars
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000 },
{ 0b10000,  // 3; s-meter, 1 bars
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000 },
{ 0b10000,  // 4; s-meter, 2 bars
  0b10000,
  0b10100,
  0b10100,
  0b10100,
  0b10100,
  0b10100,
  0b10100 },
{ 0b10000,  // 5; s-meter, 3 bars
  0b10000,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101 },
{ 0b01100,  // 6; vfo-a
  0b10010,
  0b11110,
  0b10010,
  0b10010,
  0b00000,
  0b00000,
  0b00000 },
{ 0b11100,  // 7; vfo-b
  0b10010,
  0b11100,
  0b10010,
  0b11100,
  0b00000,
  0b00000,
  0b00000 },
{ 0b00000,  // 8; TBD
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000 }
};

#ifndef VSS_METER
int analogSafeRead(uint8_t pin, bool ref1v1 = false) {  // performs classical analogRead with default Arduino sample-rate and analog reference setting; restores previous settings
	noInterrupts();
	for (; !(ADCSRA & (1 << ADIF)););  // wait until (a potential previous) ADC conversion is completed
	uint8_t adcsra = ADCSRA;
	uint8_t admux = ADMUX;
	ADCSRA &= ~(1 << ADIE);  // disable interrupts when measurement complete
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescaler for 9.6kHz
	if (ref1v1) ADMUX &= ~(1 << REFS0);  // restore reference voltage AREF (1V1)
	else ADMUX = (1 << REFS0);  // restore reference voltage AREF (5V)
	delay(1);  // settle
	int val = analogRead(pin);
	ADCSRA = adcsra;
	ADMUX = admux;
	interrupts();
	return val;
}
#else //VSS_METER
uint16_t analogSafeRead(uint8_t adcpin, bool ref1v1 = false) {
	noInterrupts();
	uint8_t oldmux = ADMUX;
	ADMUX = (3 & 0x0f) | ((ref1v1) ? (1 << REFS1) : 0) | (1 << REFS0);  // set MUX for next conversion   note: hardcoded for BUTTONS adcpin
	for (; !(ADCSRA & (1 << ADIF)););  // wait until (a potential previous) ADC conversion is completed
	delayMicroseconds(16);  // settle
	ADCSRA |= (1 << ADSC);    // start next ADC conversion
	for (; !(ADCSRA & (1 << ADIF)););  // wait until ADC conversion is completed
	ADMUX = oldmux;
	uint16_t adc = ADC;
	interrupts();
	return adc;
}
#endif

uint16_t analogSampleMic()
{
	uint16_t adc;
	noInterrupts();
	ADCSRA = (1 << ADEN) | (((uint8_t)log2((uint8_t)(F_CPU / 13 / (192307 / 1)))) & 0x07);  // hack: faster conversion rate necessary for VOX

	if ((dsp_cap == SDR) && (vox_thresh >= 32)) fastdigitalWrite(RX, LOW);  // disable RF input, only for SDR mod and with low VOX threshold
	uint8_t oldmux = ADMUX;
	for (; !(ADCSRA & (1 << ADIF)););  // wait until (a potential previous) ADC conversion is completed
	ADMUX = admux[2];  // set MUX for next conversion
	ADCSRA |= (1 << ADSC);    // start next ADC conversion
	for (; !(ADCSRA & (1 << ADIF)););  // wait until ADC conversion is completed
	ADMUX = oldmux;
	if ((dsp_cap == SDR) && (vox_thresh >= 32)) fastdigitalWrite(RX, HIGH);  // enable RF input, only for SDR mod and with low VOX threshold
	adc = ADC;
	interrupts();
	return adc;
}

volatile bool change = true;
volatile bool changedMode = 0;
volatile bool changedModeCAT = 0;
volatile int32_t freq = 14000000;
static int32_t vfo[] = { 7074000, 14074000 };
static uint8_t vfomode[] = { LSB, USB };  // G8RDI mod was USB, USB
enum vfo_t { VFOA = 0, VFOB = 1, SPLIT = 2 };
volatile uint8_t vfosel = VFOA;
volatile int32_t rit = 0;	// GW8RDI mod - changed to int32_t from int16_t
#ifdef CAT_XO_CMD
volatile int32_t tit = 0;	// GW8RDI mod - added Transit offset, used with Quantum Spectrum module
#endif
volatile uint8_t semi_qsk = false;

uint8_t smode = 1;
uint32_t max_absavg256 = 0;
int16_t dbm;

static int16_t smeter_cnt = 0;

int16_t smeter(int16_t ref = 0)
{
	max_absavg256 = max(_absavg256, max_absavg256); // peak

	if ((smode) && ((++smeter_cnt % 2048) == 0)) {   // slowed down display slightly
		float rms = (float)max_absavg256 * (float)(1 << att2);
		if (dsp_cap == SDR) rms /= (256.0 * 1024.0 * (float)R * 8.0 * 500.0 * 1.414 / (0.707 * 1.1));   // = -98.8dB  1 rx gain stage: rmsV = ADC value * AREF / [ADC DR * processing gain * receiver gain * "RMS compensation"]
		else               rms /= (256.0 * 1024.0 * (float)R * 2.0 * 100.0 * 120.0 / (1.750 * 5.0));   // = -94.6dB
		dbm = 10 * log10((rms * rms) / 50) + 30 - ref; //from rmsV to dBm at 50R

		lcd.noCursor();
		if (smode == 1) { // dBm meter
			lcd.setCursor(9, 0); lcd.print((int16_t)dbm); lcd.print(F("dBm "));
		}
		if (smode == 2) { // S-meter
			uint8_t s = (dbm < -63) ? ((dbm - -127) / 6) : (((uint8_t)(dbm - -73)) / 10) * 10;  // dBm to S (modified to work correctly above S9)
			lcd.setCursor(14, 0); if (s < 10) { lcd.print('S'); } lcd.print(s);
		}
		if (smode == 3) { // S-bar
			int8_t s = (dbm < -63) ? ((dbm - -127) / 6) : (((uint8_t)(dbm - -73)) / 10) * 10;  // dBm to S (modified to work correctly above S9)
			char tmp[5];
			for (uint8_t i = 0; i != 4; i++) { tmp[i] = max(2, min(5, s + 1)); s = s - 3; } tmp[4] = 0;
			lcd.setCursor(12, 0); lcd.print(tmp);
		}
#ifdef CW_DECODER
		if (smode == 4) { // wpm-indicator
			lcd.setCursor(14, 0); if (mode == CW) lcd.print(wpm); lcd.print("  ");
		}
#endif  //CW_DECODER
#ifdef VSS_METER
		if (smode == 5) { // Supply-voltage indicator; add resistor of value R_VSS (see below) between 12V supply input and pin 26 (PC3)   Contribution by Jeff WB4LCG: https://groups.io/g/ucx/message/4470
#define R_VSS   1000 // for 1000kOhm from VSS to PC3 (and 10kOhm to GND). Correct this value until VSS is matching
			uint8_t vss10 = (uint32_t)analogSafeRead(BUTTONS, true) * (R_VSS + 10) * 11 / (10 * 1024);   // use for a 1.1V ADC range VSS measurement
			lcd.setCursor(10, 0); lcd.print(vss10 / 10); lcd.print('.'); lcd.print(vss10 % 10); lcd.print("V ");
		}
#endif //VSS_METER
#ifdef CLOCK
		if (smode == 6) { // clock-indicator
			uint32_t _s = (millis() * 16000000ULL / F_MCU) / 1000;
			uint8_t h = (_s / 3600) % 24;
			uint8_t m = (_s / 60) % 60;
			uint8_t s = (_s) % 60;
			lcd.setCursor(8, 0); lcd.print(h / 10); lcd.print(h % 10); lcd.print(':'); lcd.print(m / 10); lcd.print(m % 10); lcd.print(':'); lcd.print(s / 10); lcd.print(s % 10); lcd.print("  ");
		}
#endif //CLOCK
		stepsize_showcursor();
		max_absavg256 /= 2;  // Implement peak hold/decay for all meter types    
	}
	return dbm;
}

void start_rx() // Start radio receiver
{
	_init = 1;
	rx_state = 0;
	func_ptr = sdr_rx_00;  //enable RX DSP/SDR
	adc_start(2, true, F_ADC_CONV * 4); admux[2] = ADMUX;  // Note that conversion-rate for TX is factors more
	if (dsp_cap == SDR) {
#ifdef SWAP_RX_IQ
		adc_start(1, !(att == 1)/*true*/, F_ADC_CONV); admux[0] = ADMUX;
		adc_start(0, !(att == 1)/*true*/, F_ADC_CONV); admux[1] = ADMUX;
#else
		adc_start(0, !(att == 1)/*true*/, F_ADC_CONV); admux[0] = ADMUX;
		adc_start(1, !(att == 1)/*true*/, F_ADC_CONV); admux[1] = ADMUX;
#endif //SWAP_RX_IQ
	}
	else { // ANALOG, DSP
		adc_start(0, false, F_ADC_CONV); admux[0] = ADMUX; admux[1] = ADMUX;
	}
	timer1_start(F_SAMP_PWM);
	timer2_start(F_SAMP_RX);
	TCCR1A &= ~(1 << COM1B1); fastdigitalWrite(KEY_OUT, LOW); // disable KEY_OUT PWM
}

int16_t _centiGain = 0;

uint8_t txdelay = 0;
uint32_t semi_qsk_timeout = 0;

// Set RX or TX mode, with RIT support and CW offset
void switch_rxtx(uint8_t tx_enable)
{
	//GW8RDI - NOTE: Best use the PA line to trigger the Spectrum mode instead of CAT as gives near instant audio control. TX RFI can interfere with RS232 as TX starts to xmit.
#ifdef CAT_TX_CMD
  if (cat_enabled && mode != CW)  // To mute Spectrum DSP audio in CW mode connect the PA line to the Spectrum module PA input.
  {
    if (tx_enable)
    {
      Serial.print("TX0;");   	// GW8RDI mod - advise going to Xmit
	  Serial.print("TX0;");		// Send again as TX can cause RFI ?????? too check needed
    }
    else
    {
        Serial.print("RX0;");   // GW8RDI mod - advise going to Rx
		Serial.print("RX0;");
    }
  }
#endif

  TIMSK2 &= ~(1 << OCIE2A);  // disable timer compare interrupt
	delayMicroseconds(20); // wait until potential RX interrupt is finalized
	noInterrupts();
#ifdef TX_DELAY
#ifdef SEMI_QSK
	if (!(semi_qsk_timeout))
#endif
		if ((txdelay) && (tx_enable) && (!(tx)) && (!(practice))) {  // key-up TX relay in advance before actual transmission
			fastdigitalWrite(RX, LOW); // TX (disable RX)
#ifdef NTX
			fastdigitalWrite(NTX, LOW);  // TX (enable TX)
#endif //NTX
#ifdef PTX
			fastdigitalWrite(PTX, HIGH);  // TX (enable TX)
#endif //PTX
			lcd.setCursor(15, 1); lcd.print('D');  // note that this enables interrupts again.
			interrupts();    //hack.. to allow delay()
			delay(F_MCU / 16000000 * txdelay);
			noInterrupts();  //end of hack
		}
#endif //TX_DELAY
	tx = tx_enable;

#ifdef CAT_XO_CMD
	if (rit || tit)
#else
	if (rit)
#endif
		display_vfo(freq);  // GW8RDI mod - update TX freq if RIT or TIT active on transmit, and restore in RX mode

	if (tx_enable)
	{
		// TX
		_centiGain = centiGain;  // backup AGC setting
#ifdef SEMI_QSK
		semi_qsk_timeout = 0;
#endif
		switch (mode)
		{
      case USB:
      case LSB: func_ptr = dsp_tx; break;
		  case CW:  func_ptr = dsp_tx_cw; break;
		  case AM:  func_ptr = dsp_tx_am; break;
		  case FM:  func_ptr = dsp_tx_fm; break;
		}
	}
	else
	{
		// RX
		if ((mode == CW) && (!(semi_qsk_timeout))) {
#ifdef SEMI_QSK
#ifdef KEYER
			semi_qsk_timeout = millis() + ditTime * 8;
#else
			semi_qsk_timeout = millis() + 8 * 8;  // no keyer? assume dit-time of 20 WPM
#endif //KEYER
#endif //SEMI_QSK
			if (semi_qsk) func_ptr = dummy; else func_ptr = sdr_rx_00;
		}
		else {
			centiGain = _centiGain;  // restore AGC setting
#ifdef SEMI_QSK
			semi_qsk_timeout = 0;
#endif
			func_ptr = sdr_rx_00;
		}
	}

	if ((!dsp_cap) && (!tx_enable) && vox)
		func_ptr = dummy; //hack: for SSB mode, disable dsp_rx during vox mode enabled as it slows down the vox loop too much!
	interrupts();
	if (tx_enable) ADMUX = admux[2];
	else _init = 1;
	rx_state = 0;
#ifdef CW_DECODER
	if ((cwdec) && (mode == CW)) { filteredstate = tx_enable; dec2(); }
#endif  //CW_DECODER

	if (tx_enable)
	{
		// TX

		if (practice) {
			fastdigitalWrite(RX, LOW); // TX (disable RX)
			lcd.setCursor(15, 1); lcd.print('P');
			si5351.SendRegister(SI_CLK_OE, TX0RX0);		// Do not enable PWM (KEY_OUT), do not enable CLK2 - DISABLE CLK2 which is the TX PA gate clock
		}
		else
		{
			fastdigitalWrite(RX, LOW); // TX (disable RX)

#ifdef NTX
			fastdigitalWrite(NTX, LOW);  // TX (enable TX)
#endif //NTX
#ifdef PTX
			fastdigitalWrite(PTX, HIGH);  // TX (enable TX)
#endif //PTX

			lcd.setCursor(15, 1); lcd.print('T');   // Show Transmitting on LCD

			if (mode == CW)
			{
				si5351.freq_calc_fast(-cw_offset); si5351.SendPLLRegisterBulk();
			} // for CW, TX at freq
#ifdef RIT_ENABLE   // GW8RDI mod - restore freq
			else
			{
#ifdef CAT_XO_CMD
				if (rit || tit)
					si5351.freq_calc_fast(tit); si5351.SendPLLRegisterBulk();	// Restore PLL from RIT offset and add TIT offset
#else
				if (rit)
					si5351.freq_calc_fast(0); si5351.SendPLLRegisterBulk();	// Restore PLL from RIT offset
#endif
			}
#endif //RIT_ENABLE
			si5351.SendRegister(SI_CLK_OE, TX1RX0);
			OCR1AL = 0x80; // make sure SIDETONE is set at 2.5V
			if ((!mox) && (mode != CW)) TCCR1A &= ~(1 << COM1A1); // disable SIDETONE, prevent interference during SSB TX
			TCCR1A |= (1 << COM1B1);  // enable KEY_OUT PWM
#ifdef _SERIAL
			if (cat_active) { DDRC &= ~(1 << 2); } // disable PC2, so that ADC2 can be used as mic input
#endif
		}
	}
	else
	{
		// RX

#ifdef KEY_CLICK
		if (OCR1BL != 0) {
			for (uint16_t i = 0; i != 31; i++) {   // ramp down of amplitude: soft falling edge to prevent key clicks
				OCR1BL = lut[pgm_read_byte_near(ramp[i])];
				delayMicroseconds(60);
			}
		}
#endif //KEY_CLICK
		TCCR1A |= (1 << COM1A1);  // enable SIDETONE (was disabled to prevent interference during ssb tx)
		TCCR1A &= ~(1 << COM1B1); fastdigitalWrite(KEY_OUT, LOW); // disable KEY_OUT PWM, prevents interference during RX
		OCR1BL = 0; // make sure PWM (KEY_OUT) is set to 0%
#ifdef QUAD
		if (quad_enabled)  // G8RDI mod - added - keep disabled else TX voice quality is distorted
		{
#ifdef TX_CLK0_CLK1
			si5351.SendRegister(16, 0x0f);  // disable invert on CLK0
			si5351.SendRegister(17, 0x0f);  // disable invert on CLK1
#else
			si5351.SendRegister(18, 0x0f);  // disable invert on CLK2
#endif  //TX_CLK0_CLK1
		}
#endif //QUAD
		si5351.SendRegister(SI_CLK_OE, TX0RX1);
#ifdef SEMI_QSK
		if ((!semi_qsk_timeout) || (!semi_qsk))   // enable RX when no longer in semi-qsk phase; so RX and NTX/PTX outputs are switching only when in RX mode
#endif //SEMI_QSK
		{
			fastdigitalWrite(RX, !(att == 2)); // RX (enable RX when attenuator not on)
#ifdef NTX
			fastdigitalWrite(NTX, HIGH);  // RX (disable TX)
#endif //NTX
#ifdef PTX
			fastdigitalWrite(PTX, LOW);   // TX (disable TX)
#endif //PTX
		}

#ifdef RIT_ENABLE
		si5351.freq_calc_fast(rit); si5351.SendPLLRegisterBulk();  // restore original PLL RX frequency
#else
		si5351.freq_calc_fast(0); si5351.SendPLLRegisterBulk();  // restore original PLL RX frequency
#endif //RIT_ENABLE
#ifdef SWR_METER
		if (swrmeter > 0) { show_banner(); lcd.print("                "); }
#endif
		lcd.setCursor(15, 1); lcd.print((vox) ? 'V' : 'R');
#ifdef _SERIAL
		if (!vox) if (cat_active) { DDRC |= (1 << 2); } // enable PC2, so that ADC2 is pulled-down so that CAT TX is not disrupted via mic input
#endif
	}
	OCR2A = ((F_CPU / 64) / ((tx_enable) ? F_SAMP_TX : F_SAMP_RX)) - 1;
	TIMSK2 |= (1 << OCIE2A);  // enable timer compare interrupt TIMER2_COMPA_vect
}

uint8_t rx_ph_q = 90;

#ifdef QCX
#define CAL_IQ 1
#ifdef CAL_IQ
int16_t cal_iq_dummy = 0;
// RX I/Q calibration procedure: terminate with 50 ohm, enable CW filter, adjust R27, R24, R17 subsequently to its minimum side-band rejection value in dB
void calibrate_iq()
{
	smode = 1;
	lcd.setCursor(0, 0); lcd_blanks(); lcd_blanks();
	fastdigitalWrite(SIG_OUT, true); // loopback on
	si5351.freq(freq, 0, 90);  // RX in USB  
	si5351.SendRegister(SI_CLK_OE, TX1RX1);
	float dbc;
	si5351.freqb(freq + 700); delay(100);
	dbc = smeter();
	si5351.freqb(freq - 700); delay(100);
	lcd.setCursor(0, 1); lcd.print("I-Q bal. 700Hz"); lcd_blanks();
	for (; !_digitalRead(BUTTONS);) { wdt_reset(); smeter(dbc); } for (; _digitalRead(BUTTONS);) wdt_reset();
	si5351.freqb(freq + 600); delay(100);
	dbc = smeter();
	si5351.freqb(freq - 600); delay(100);
	lcd.setCursor(0, 1); lcd.print("Phase Lo 600Hz"); lcd_blanks();
	for (; !_digitalRead(BUTTONS);) { wdt_reset(); smeter(dbc); } for (; _digitalRead(BUTTONS);) wdt_reset();
	si5351.freqb(freq + 800); delay(100);
	dbc = smeter();
	si5351.freqb(freq - 800); delay(100);
	lcd.setCursor(0, 1); lcd.print("Phase Hi 800Hz"); lcd_blanks();
	for (; !_digitalRead(BUTTONS);) { wdt_reset(); smeter(dbc); } for (; _digitalRead(BUTTONS);) wdt_reset();

	lcd.setCursor(9, 0); lcd_blanks();  // cleanup dbmeter
	fastdigitalWrite(SIG_OUT, false); // loopback off
	si5351.SendRegister(SI_CLK_OE, TX0RX1);
	change = true;  //restore original frequency setting
}
#endif
#endif //QCX

uint8_t prev_bandval = 3;
uint8_t bandval = 3;

#define N_BANDS 11  // See KEEP_BAND_DATA if more than 9 bands required.

#ifdef CW_FREQS_QRP
uint32_t band[N_BANDS] = { /*472000,*/ 1810000, 3560000, 5351500, 7030000, 10106000, 14060000, 18096000, 21060000, 24906000, 28060000, 50096000/*, 70160000, 144060000*/ };  // CW QRP freqs
#else
#ifdef CW_FREQS_FISTS
uint32_t band[N_BANDS] = { /*472000,*/ 1818000, 3558000, 5351500, 7028000, 10118000, 14058000, 18085000, 21058000, 24908000, 28058000, 50058000/*, 70158000, 144058000*/ };  // CW FISTS freqs
#else
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 03/01/2026 - 9M2GRC
////////////////////////////////////////////////////////////////////////
#ifdef TRUSDX	
#ifdef LOBAND
// Band Selection:                              80m      60m      40m      30m       20m
uint32_t band[N_BANDS] = { /*472000,*/ 1840000, 3573000, 5357000, 7074000, 10136000, 14074000, 18100000, 21074000, 24915000, 28074000, 50313000/*, 70101000, 144125000*/ };  // FT8 freqs
#endif 
#ifdef CLASSICBAND
// Band Selection:                              80m      40m      20m       15m       10m  
uint32_t band[N_BANDS] = { /*472000,*/ 1840000, 3573000, 7074000, 14074000, 21074000, 28074000, 50125000, 144125000, 222125000, 420125000, 462563000/*, 70101000, 144125000*/ };
#endif
#ifdef HIBAND
// Band Selection:                              20m       17m       15m       12m       10m
uint32_t band[N_BANDS] = { /*472000,*/ 1840000, 14074000, 18100000, 21074000, 24915000, 28074000, 50125000, 144125000, 222125000, 420125000, 462563000/*, 70101000, 144125000*/ };
#endif 
#else 
uint32_t band[N_BANDS] = { /*472000,*/ 1840000, 3573000, 5357000, 7074000, 10136000, 14074000, 18100000, 21074000, 24915000, 28074000, 50313000/*, 70101000, 144125000*/ };  // FT8 freqs
#endif
#endif
#endif // TRUSDX

enum step_t { STEP_10M, STEP_1M, STEP_500k, STEP_100k, STEP_10k, STEP_1k, STEP_500, STEP_100, STEP_10, STEP_1 };
uint32_t stepsizes[10] = { 10000000, 1000000, 500000, 100000, 10000, 1000, 500, 100, 10, 1 };
volatile uint8_t stepsize = STEP_1k;
uint8_t prev_stepsize[] = { STEP_1k, STEP_500 }; //default stepsize for resp. SSB, CW

#ifdef KEEP_BAND_DATA  // G8RDI mod - Up to 9 bands are supported of 11. To increase change code.
#define BANDCOUNT N_BANDS-2
static int32_t freq_last[BANDCOUNT];  // 0-8 Last freq used on each band
static uint8_t mode_last[BANDCOUNT];  // Last mode used
#endif

void process_encoder_tuning_step(int8_t steps)
{
	int32_t stepval = stepsizes[stepsize];
	//if(stepsize < STEP_100) freq %= 1000; // when tuned and stepsize > 100Hz then forget fine-tuning details
	if (rit) {
		rit += steps * stepval;
		rit = max(-99999, min(99999, rit));	// GW8RDI - mod changed to support +/- 99 KHz RIT receiver offset
	}
	else {
		freq += steps * stepval;
		freq = max(1, min(999999999, freq));
	}
	change = true;
}

void stepsize_showcursor()
{
	lcd.setCursor(stepsize + 1, 1);  // display stepsize with cursor
	lcd.cursor();
}

void stepsize_change(int8_t val)
{
	stepsize += val;
	if (stepsize < STEP_1M) stepsize = STEP_10;
	if (stepsize > STEP_10) stepsize = STEP_1M;
	if (stepsize == STEP_10k || stepsize == STEP_500k) stepsize += val;
	stepsize_showcursor();
}

void powerDown()
{ // Reduces power from 110mA to 70mA (back-light on) or 30mA (back-light off), remaining current is probably opamp quiescent current
	lcd.setCursor(0, 1); lcd.print(F("Power-off 73 :-)")); lcd_blanks();

	MCUSR = ~(1 << WDRF);  // MSY be done before wdt_disable()
	wdt_disable();   // WDTON Fuse High bit need to be 1 (0xD1), if NOT it will override and set WDE=1; WDIE=0, meaning MCU will reset when watchdog timer is zero, and this seems to happen when wdt_disable() is called

	timer2_stop();
	timer1_stop();
	adc_stop();

	si5351.powerDown();

	delay(1500);

	// Disable external interrupts INT0, INT1, Pin Change
	PCICR = 0;
	PCMSK0 = 0;
	PCMSK1 = 0;
	PCMSK2 = 0;
	// Disable internal interrupts
	TIMSK0 = 0;
	TIMSK1 = 0;
	TIMSK2 = 0;
	WDTCSR = 0;
	// Enable BUTTON Pin Change interrupt
	*digitalPinToPCMSK(BUTTONS) |= (1 << digitalPinToPCMSKbit(BUTTONS));
	*digitalPinToPCICR(BUTTONS) |= (1 << digitalPinToPCICRbit(BUTTONS));

	// Power-down sub-systems
	PRR = 0xff;

	lcd.noDisplay();
	PORTD &= ~BACKLIGHT_PIN; // disable backlight

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	interrupts();
	sleep_bod_disable();
	sleep_cpu();  // go to sleep mode, wake-up by either INT0, INT1, Pin Change, TWI Addr Match, WDT, BOD
	sleep_disable();

	do { wdt_enable(WDTO_15MS); for (;;); } while (0);  // soft reset by trigger watchdog timeout
}

char* szStation = (char*)MY_CALLSIGN_PADDED;  // If callsign is different length, change [5] and [6] below to match 2 spaces at end.

void show_banner() {
	lcd.setCursor(0, 0);
#ifdef QCX
	lcd.print(F("QCX"));
	const char* cap_label[] = { "SSB", "DSP", "SDR" };
	if (ssb_cap || dsp_cap) { lcd.print('-'); lcd.print(cap_label[dsp_cap]); }
#else
	if (quad_enabled)
		szStation[CALLSIGN_LENGTH] = 'q';
	else
		szStation[CALLSIGN_LENGTH] = ' ';
	if (cat_enabled)
		szStation[CALLSIGN_LENGTH + 1] = 'c';
	else
		szStation[CALLSIGN_LENGTH + 1] = ' ';

#ifdef DEBUG_G8RDI  // DEBUG DISPLAY HEX VALUE - disable on release code
	sprintf(&szStation[0], "%04X", fir_value);
#else
	if (error_code > 0)
		sprintf(&szStation[CALLSIGN_LENGTH], "%02X", error_code);     // G8RDI mod - added
#endif

	lcd.print(szStation);   // "uSDX"
#endif //QCX
	lcd.print('\x01'); lcd_blanks(); lcd_blanks();
}

const char* vfosel_label[] = { "A", "B"/*, "Split"*/ };
const char* mode_label[5] = { "LSB", "USB", "CW ", "FM ", "AM " };

// Display frequency on LCD.  If RIT is enabled, displays just the receiver offset unless in TX
inline void display_vfo(int32_t f)
{
	lcd.setCursor(0, 1);

	lcd.print((rit) ? ' ' : ((vfosel % 2) | ((vfosel == SPLIT) & tx)) ? '\x07' : '\x06');  // RIT, VFO A/B

#ifdef CAT_XO_CMD
	if (tx && tit != 0)	// GW8RDI mod - TX offset
		f += tit;
#endif

	int32_t scale = 10e6;

	if (rit && !tx)   // GW8RDI mod - In TX mode always show TX freq.
	{
		f = rit;      // Show RIT RX offset only
		scale = 1e3;  // RIT frequency
		lcd.print(F("RIT")); lcd.print(rit < 0 ? '-' : '+');		// GW8RDI mod - changinged from RIT +/- 10 KHz, to +/- 50 KHz so need that digit!
	}
	else
	{
		if (f / scale == 0) { lcd.print(' '); scale /= 10; }  // Initial space instead of zero
	}
	for (; scale != 1; f %= scale, scale /= 10) {
		lcd.print(abs(f / scale));
		if (scale == (int32_t)1e3 || scale == (int32_t)1e6) lcd.print(',');  // Thousands separator
	}

	lcd.print(' '); lcd.print(mode_label[mode]); lcd.print(' ');
	lcd.setCursor(15, 1); lcd.print((vox) ? 'V' : 'R');
}

volatile uint8_t event;
volatile uint8_t prev_menumode = 0;
volatile int8_t menu = 0;  // current parameter id selected in menu

#define pgm_cache_item(addr, sz) byte _item[sz]; memcpy_P(_item, addr, sz);  // copy array item from PROGMEM to SRAM
#define get_version_id() ((VERSION[0]-'1') * 2048 + ((VERSION[2]-'0')*10 + (VERSION[3]-'0')) * 32 +  ((VERSION[4]) ? (VERSION[4] - 'a' + 1) : 0) * 1)  // converts VERSION string with (fixed) format "9.99z" into uint16_t (max. values shown here, z may be removed) 

uint16_t eeprom_version;
#define EEPROM_OFFSET 0x150  // avoid collision with QCX settings, overwrites text settings though
int eeprom_addr;

// Support functions for parameter and menu handling
enum action_t { UPDATE, UPDATE_MENU, NEXT_MENU, LOAD, SAVE, SKIP, NEXT_CH };

// output menuid in x.y format
void printmenuid(uint8_t menuid) {
	static const char seperator[] = { '.', ' ' };
	uint8_t ids[] = { (uint8_t)(menuid >> 4), (uint8_t)(menuid & 0xF) };
	for (int i = 0; i < 2; i++) {
		uint8_t id = ids[i];
		if (id >= 10) {
			id -= 10;
			lcd.print('1');
		}
		lcd.print(char('0' + id));
		lcd.print(seperator[i]);
	}
}

void printlabel(uint8_t action, uint8_t menuid, const __FlashStringHelper * label) {
	if (action == UPDATE_MENU) {
		lcd.setCursor(0, 0);
		printmenuid(menuid);
		lcd.print(label); lcd_blanks(); lcd_blanks();
		lcd.setCursor(0, 1); // value on next line
		if (menumode >= 2) lcd.print('>');
	}
	else { // UPDATE (not in menu)
		lcd.setCursor(0, 1); lcd.print(label); lcd.print(F(": "));
	}
}

void actionCommon(uint8_t action, uint8_t * ptr, uint8_t size) {
	switch (action) {
	case LOAD:
		eeprom_read_block((void*)ptr, (const void*)eeprom_addr, size);
		break;
	case SAVE:
		eeprom_write_block((const void*)ptr, (void*)eeprom_addr, size);
		break;
	case SKIP:
		break;
	}
	eeprom_addr += size;
}

template<typename T> void paramAction(uint8_t action, volatile T & value, uint8_t menuid, const __FlashStringHelper * label, const char* enumArray[], int32_t _min, int32_t _max, bool continuous) {
	switch (action) {
	case UPDATE:
	case UPDATE_MENU:
		if (((int32_t)value + encoder_val) < _min) value = (continuous) ? _max : _min;
		else if (((int32_t)value + encoder_val) > _max) value = (continuous) ? _min : _max;
		else value = (int32_t)value + encoder_val;
		encoder_val = 0;

		lcd.noCursor();
		printlabel(action, menuid, label);  // print normal/menu label
		if (enumArray == NULL) {  // print value
			if ((_min < 0) && (value >= 0)) lcd.print('+');  // add + sign for positive values, in case negative values are supported
			lcd.print(value);
		}
		else {
			lcd.print(enumArray[value]);
		}
		lcd_blanks(); lcd_blanks(); //lcd.setCursor(0, 1);
		break;
	default:
		actionCommon(action, (uint8_t*)&value, sizeof(value));
		break;
	}
}

#ifdef MENU_STR
static uint8_t pos = 0;
void paramAction(uint8_t action, char* value, uint8_t menuid, const __FlashStringHelper * label, uint8_t size) {
	const uint8_t _min = ' '; const uint8_t _max = 'Z';
	switch (action) {
	case NEXT_CH:
		if (pos < size) pos++;  // allow to go to next character when string size allows and when current character is not string end
		action = UPDATE_MENU; //fall-through next case
	case UPDATE:
	case UPDATE_MENU:
		if (menumode != 3) pos = 0;
		if (menumode == 2) menumode = 3; // hack: for strings enter in edit mode
		if (((value[pos] + encoder_val) < _min) || ((value[pos] + encoder_val) == 0)) value[pos] = _min;
		else if ((value[pos] + encoder_val) > _max) value[pos] = _max;
		else value[pos] = value[pos] + encoder_val;
		encoder_val = 0;

		printlabel(action, menuid, label);  // print normal/menu label
		for (int i = 0; i != 13; i++) { char ch = value[(pos / 8) * 8 + i]; if (ch) lcd.print(ch); else break; } // print value
		lcd.print('\x01');  // print terminator
		lcd_blanks();
		lcd.setCursor((pos % 8) + (menumode >= 2), 1); lcd.cursor();
		break;
	case SAVE:
		for (uint8_t i = size; i > 0; i--) {
			if ((value[i - 1] == ' ') || (value[i - 1] == 0)) value[i - 1] = 0;  // remove trailing spaces
			else break; // stop once content found
		}
	default:
		actionCommon(action, (uint8_t*)value, size);
		break;
	}
}
#endif //MENU_STR

static uint32_t save_event_time = 0;
static uint8_t vox_tx = 0;
static uint8_t vox_sample = 0;
static uint16_t vox_adc = 0;

static uint8_t pwm_min = 0;    // PWM value for which PA reaches its minimum: 29 when C31 installed;   0 when C31 removed;   0 for biasing BS170 directly
#ifdef QCX
static uint8_t pwm_max = 255;  // PWM value for which PA reaches its maximum: 96 when C31 installed; 255 when C31 removed;
#else
static uint8_t pwm_max = 160;  // PWM value for which PA reaches its maximum:  128 for biasing BS170 directly, 160 for IRFI510G
#endif

const char* offon_label[2] = { "OFF", "ON" };
#if(F_MCU > 16000000)
const char* filt_label[N_FILT + 1] = { "Full", "3000", "2400", "1800", "500", "200", "100", "50" };
#else
const char* filt_label[N_FILT + 1] = { "Full", "2400", "2000", "1500", "500", "200", "100", "50" };
#endif
#ifdef NR_FIR
const int filt_val[N_FILT + 1] = { 3000, 2700, 2200, 1800, 400, 150, 80, 30 };  // GW8RDI mod
#endif

#ifdef TRUSDX
#ifdef LOBAND
const char* band_label[N_BANDS] = { "x", "80m", "60m", "40m", "30m", "20m", "x" };  // G8RDI mod - squeezing out every free byte!
#endif 
#ifdef CLASSICBAND
const char* band_label[N_BANDS] = { "x", "80m", "40m", "20m", "15m", "10m", "x", "x", "x", "x", "x" };
#endif
#ifdef HIBAND
const char* band_label[N_BANDS] = { "x", "20m", "17m", "15m", "12m", "10m", "x", "x", "x", "x", "x" };  
#endif
#else 
const char* band_label[N_BANDS] = { "x", "80m", "60m", "40m", "30m", "20m", "17m", "15m", "12m", "10m", "x" };  // G8RDI mod - squeezing out every free byte!
#endif // TRUSDX

const char* stepsize_label[] = { "10M", "1M", ".5M", "100k", "10k", "1k", ".5k", "100", "10", "1" };  // GW8RDI 0 b4 0. removed to save memory
const char* att_label[] = { "0dB", "-13dB", "-20dB", "-33dB", "-40dB", "-53dB", "-60dB", "-73dB" };
#ifdef CLOCK
const char* smode_label[] = { "OFF", "dBm", "S", "Sbar", "wpm", "Vss", "time" };
#else
#ifdef VSS_METER
const char* smode_label[] = { "OFF", "dBm", "S", "Sbar", "wpm", "Vss" };
#else
const char* smode_label[] = { "OFF", "dBm", "S", "Sbar", "wpm" };
#endif
#endif
#ifdef SWR_METER
const char* swr_label[] = { "OFF", "FwdSWR", "FwdRef", "VFwdVREF" };  // GW8RDI mod - byte saving
#endif
const char* cw_tone_label[] = { "700", "600" };
#ifdef KEYER
const char* keyer_mode_label[] = { "IambicA", "IambicB","Straight" };  // GW8RDI mod - byte saving was "Iambic A"
#endif
const char* agc_label[] = { "OFF", "Fast", "Slow" };

#define _N(a) sizeof(a)/sizeof(a[0])

#define N_PARAMS 44+3  // number of (visible) parameters  // G8RDI mod +3 for added visible menu items
#ifdef KEEP_BAND_DATA
#define I_PARAMS 5+9
enum params_t { _NULL, VOLUME, MODE, FILTER, BAND, STEP, VFOSEL, RIT, AGC, NR, ATT, ATT2, SMETER, SWRMETER, CWDEC, CWTONE, CWOFF, SEMIQSK, KEY_WPM, KEY_MODE, KEY_PIN, KEY_TX, TONE_VOL, VOX, VOXGAIN, DRIVE, TXDELAY, MOX, CWINTERVAL, CWMSG1, CWMSG2, CWMSG3, CWMSG4, CWMSG5, CWMSG6, PWM_MIN, PWM_MAX, SIFXTAL, IQ_ADJ, CAT_ACTIVE, QUAD_ACTIVE, CALIB, SR, CPULOAD, PARAM_A, PARAM_B, PARAM_C, BACKL, FREQA, FREQB, MODEA, MODEB, VERS, BAND_DATA0, BAND_DATA1, BAND_DATA2, BAND_DATA3, BAND_DATA4, BAND_DATA5, BAND_DATA6, BAND_DATA7, BAND_DATA8, ALL = 0xff };
#else
#define I_PARAMS 5
enum params_t { _NULL, VOLUME, MODE, FILTER, BAND, STEP, VFOSEL, RIT, AGC, NR, ATT, ATT2, SMETER, SWRMETER, CWDEC, CWTONE, CWOFF, SEMIQSK, KEY_WPM, KEY_MODE, KEY_PIN, KEY_TX, TONE_VOL, VOX, VOXGAIN, DRIVE, TXDELAY, MOX, CWINTERVAL, CWMSG1, CWMSG2, CWMSG3, CWMSG4, CWMSG5, CWMSG6, PWM_MIN, PWM_MAX, SIFXTAL, IQ_ADJ, CAT_ACTIVE, QUAD_ACTIVE, CALIB, SR, CPULOAD, PARAM_A, PARAM_B, PARAM_C, BACKL, FREQA, FREQB, MODEA, MODEB, VERS, ALL = 0xff };
#endif
#define N_ALL_PARAMS (N_PARAMS+I_PARAMS)  // number of parameters

int8_t paramAction(uint8_t action, uint8_t id = ALL)  // list of parameters
{
	if ((action == SAVE) || (action == LOAD)) {
		eeprom_addr = EEPROM_OFFSET;
		for (uint8_t _id = 1; _id < id; _id++) paramAction(SKIP, _id);
	}
	if (id == ALL) for (id = 1; id != N_ALL_PARAMS + 1; id++) paramAction(action, id);  // for all parameters

	switch (id) {    // Visible parameters
	case VOLUME:  paramAction(action, volume, 0x11, F("Vol"), NULL, -1, 16, false); break;  // GW8RDI mod - "Volume"
	case MODE:    paramAction(action, mode, 0x12, F("Mode"), mode_label, 0, _N(mode_label) - 1, false); break;
	case FILTER:  paramAction(action, filt, 0x13, F("FilterBW"), filt_label, 0, _N(filt_label) - 1, false); break;
#ifdef TRUSDX
	case BAND:    paramAction(action, bandval, 0x14, F("Band"), band_label, 1, _N(band_label) - 6, false); break;
#else
	case BAND:    paramAction(action, bandval, 0x14, F("Band"), band_label, 1, _N(band_label) - 2, false); break;  // G8RDI mod - changed min to 1 as 160M not in use, and _N(band_label) - 1 to -2 as 6m also
#endif
	case STEP:    paramAction(action, stepsize, 0x15, F("Tune Rate"), stepsize_label, 0, _N(stepsize_label) - 1, false); break;
	case VFOSEL:  paramAction(action, vfosel, 0x16, F("VFO Mode"), vfosel_label, 0, _N(vfosel_label) - 1, false); break;
#ifdef RIT_ENABLE
	case RIT:     paramAction(action, rit, 0x17, F("RIT"), offon_label, 0, 1, false); break;
#endif
#ifdef FAST_AGC
	case AGC:     paramAction(action, agc, 0x18, F("AGC"), agc_label, 0, _N(agc_label) - 1, false); break;
#else
	case AGC:     paramAction(action, agc, 0x18, F("AGC"), offon_label, 0, 1, false); break;
#endif // FAST_AGC
#ifdef NR_FIR
	case NR:
		if ((int32_t)nr + encoder_val < 3)
			paramAction(action, nr, 0x19, F("NR"), NULL, 0, 8, false);
		else
			paramAction(action, nr, 0x19, F("DSP NR"), NULL, 0, 8, false);
#else
	case NR:      paramAction(action, nr, 0x19, F("NR"), NULL, 0, 8, false);
#endif
#ifdef NR_FIR
		if (nr > 2)
			FirFilterSetup(7 + (((nr - 2) - 1) * 2), filt_val[filt], F_SAMP_RX / 8);  // GW8RDI mod
#endif
		break;
	case ATT:     paramAction(action, att, 0x1A, F("ATT"), att_label, 0, 7, false); break;
	case ATT2:    paramAction(action, att2, 0x1B, F("ATT2"), NULL, 0, 16, false); break;
	case SMETER:  paramAction(action, smode, 0x1C, F("S-Meter"), smode_label, 0, _N(smode_label) - 1, false); break;
#ifdef SWR_METER
	case SWRMETER:  paramAction(action, swrmeter, 0x1D, F("SWR Meter"), swr_label, 0, _N(swr_label) - 1, false); break;
#endif
#ifdef CW_DECODER
	case CWDEC:   paramAction(action, cwdec, 0x21, F("CW Decoder"), offon_label, 0, 1, false); break;
#endif
#ifdef FILTER_700HZ
	case CWTONE:  if (dsp_cap) paramAction(action, cw_tone, 0x22, F("CW Tone"), cw_tone_label, 0, 1, false); break;
#endif
#ifdef QCX
	case CWOFF:   paramAction(action, cw_offset, 0x23, F("CW Off"), NULL, 300, 2000, false); break; // GW8RDI mod save buyes "CW Offset"
#endif
#ifdef SEMI_QSK
	case SEMIQSK: paramAction(action, semi_qsk, 0x24, F("Semi QSK"), offon_label, 0, 1, false); break;
#endif
#if defined(KEYER) || defined(CW_MESSAGE)
	case KEY_WPM:  paramAction(action, keyer_speed, 0x25, F("Keyer Speed"), NULL, 1, 60, false); break;
#endif
#ifdef KEYER
	case KEY_MODE: paramAction(action, keyer_mode, 0x26, F("Keyer Mode"), keyer_mode_label, 0, 2, false); break;
	case KEY_PIN:  paramAction(action, keyer_swap, 0x27, F("Keyer Swap"), offon_label, 0, 1, false); break;
#endif
	case KEY_TX:   paramAction(action, practice, 0x28, F("Practice"), offon_label, 0, 1, false); break;
#ifdef CW_VOLUME
	case TONE_VOL: paramAction(action, tone_vol, 0x29, F("Tone Vol"), NULL, 0, 16, false); break;
#endif
#ifdef VOX_ENABLE
	case VOX:     paramAction(action, vox, 0x31, F("VOX"), offon_label, 0, 1, false); break;
	case VOXGAIN: paramAction(action, vox_thresh, 0x32, F("Noise Gate"), NULL, 0, 255, false); break; // GW8RDI mod - "Noise Gate" save bytes
#endif
	case DRIVE:   paramAction(action, drive, 0x33, F("TX Drive"), NULL, 0, 8, false); break;
#ifdef TX_DELAY
	case TXDELAY: paramAction(action, txdelay, 0x34, F("TX Delay"), NULL, 0, 255, false); break;
#endif
#ifdef MOX_ENABLE
	case MOX:     paramAction(action, mox, 0x35, F("MOX"), NULL, 0, 2, false); break;
#endif
#ifdef CW_MESSAGE
	case CWINTERVAL: paramAction(action, cw_msg_interval, 0x41, F("CQ Interval"), NULL, 0, 60, false); break;
#ifdef CW_MESSAGE_EXT
	case CWMSG1:    paramAction(action, cw_msg[0], 0x42, F("CQ Msg 1"), sizeof(cw_msg)); break;
	case CWMSG2:    paramAction(action, cw_msg[1], 0x43, F("CW Msg 2"), sizeof(cw_msg)); break;
	case CWMSG3:    paramAction(action, cw_msg[2], 0x44, F("CW Msg 3"), sizeof(cw_msg)); break;
	case CWMSG4:    paramAction(action, cw_msg[3], 0x45, F("CW Msg 4"), sizeof(cw_msg)); break;
	case CWMSG5:    paramAction(action, cw_msg[4], 0x46, F("CW Msg 5"), sizeof(cw_msg)); break;
	case CWMSG6:    paramAction(action, cw_msg[5], 0x47, F("CW Msg 6"), sizeof(cw_msg)); break;
#else
	case CWMSG1:    paramAction(action, cw_msg[0], 0x42, F("CQ Msg"), sizeof(cw_msg)); break;
#endif
#endif
	case PWM_MIN: paramAction(action, pwm_min, 0x81, F("PA bias min"), NULL, 0, pwm_max - 1, false); break;
	case PWM_MAX: paramAction(action, pwm_max, 0x82, F("PA max"), NULL, pwm_min, 255, false); break;
	case SIFXTAL: paramAction(action, si5351.fxtal, 0x83, F("Ref frq"), NULL, 14000000, 28000000, false); break;
	case IQ_ADJ:  paramAction(action, rx_ph_q, 0x84, F("IQ phase"), NULL, 0, 180, false); break;
#ifdef CAL_IQ
	case CALIB:   if (dsp_cap != SDR) paramAction(action, cal_iq_dummy, 0x85, F("IQ Test/Cal."), NULL, 0, 0, false); break;
#endif
#ifdef CAT
#if defined(CAT_FAST) || defined(CAT_STREAMING)
	case CAT_ACTIVE: paramAction(action, cat_enabled, 0x86, F("CAT115K"), offon_label, 0, 1, false);       // CAT115K2-81N
  if (cat_enabled)  // G8RDI mod 230401
	{
		Serial.begin(16000000ULL * 115200 / F_MCU); // corrected for F_CPU=20M
		Command_IF();
#if !defined(OLED) && defined(TESTBENCH)
		smode = 0;  // In case of LCD, turn off smeter
#endif
	}
  break;
#else
	case CAT_ACTIVE: paramAction(action, cat_enabled, 0x86, F("CAT38K"), offon_label, 0, 1, false);   // CAT38K4-81N
  if (cat_enabled)  // G8RDI mod 230401
	{
		Serial.begin(16000000ULL * 38400 / F_MCU); // corrected for F_CPU=20M
		Command_IF();
#if !defined(OLED) && defined(TESTBENCH)
		smode = 0;  // In case of LCD, turn off smeter
#endif
	}
  break;
#endif
#endif
#ifdef QUAD
	case QUAD_ACTIVE: paramAction(action, quad_enabled, 0x87, F("QUAD"), offon_label, 0, 1, false); break;
#endif
#ifdef DEBUG
	case SR:      paramAction(action, sr, 0x91, F("Sample rate"), NULL, INT32_MIN, INT32_MAX, false); break;
	case CPULOAD: paramAction(action, cpu_load, 0x92, F("CPU load %"), NULL, INT32_MIN, INT32_MAX, false); break;
	case PARAM_A: paramAction(action, param_a, 0x93, F("ParamA"), NULL, 0, UINT16_MAX, false); break;
	case PARAM_B: paramAction(action, param_b, 0x94, F("ParamB"), NULL, INT16_MIN, INT16_MAX, false); break;
	case PARAM_C: paramAction(action, param_c, 0x95, F("ParamC"), NULL, INT16_MIN, INT16_MAX, false); break;
#endif
	case BACKL:   paramAction(action, backlight, 0xA1, F("Light"), offon_label, 0, 1, false); break;   // GW8RDI "Backlight" workaround for varying N_PARAM and not being able to overflowing default cases properly
		// Invisible parameters
	case FREQA:   paramAction(action, vfo[VFOA], 0, NULL, NULL, 0, 0, false); break;
	case FREQB:   paramAction(action, vfo[VFOB], 0, NULL, NULL, 0, 0, false); break;
	case MODEA:   paramAction(action, vfomode[VFOA], 0, NULL, NULL, 0, 0, false); break;
	case MODEB:   paramAction(action, vfomode[VFOB], 0, NULL, NULL, 0, 0, false); break;
	case VERS:    paramAction(action, eeprom_version, 0, NULL, NULL, 0, 0, false); break;
		// Non-parameters
	case _NULL:   menumode = 0; show_banner(); change = true; break;

	default:
  #ifdef KEEP_BAND_DATA // 230401 freed 258 bytes!!
  if (id >= BAND_DATA0 && id <= BAND_DATA8)
  {
    uint8_t is = id - BAND_DATA0;
    paramAction(action, freq_last[is], 0, NULL, NULL, 0, 0, false);
		paramAction(action, mode_last[is], 0, NULL, NULL, 0, 0, false); break;
  }
  #endif

  if ((action == NEXT_MENU) && (id != N_PARAMS))
    id = paramAction(action, max(1 /*0*/, min(N_PARAMS, id + ((encoder_val > 0) ? 1 : -1))));
  break;  // keep iterating util menu item found
	}
	return id;
}

void initPins() {
	// initialize
	fastdigitalWrite(SIG_OUT, LOW);
	fastdigitalWrite(RX, HIGH);
	fastdigitalWrite(KEY_OUT, LOW);
	fastdigitalWrite(SIDETONE, LOW);

	// pins
	fastpinMode(SIDETONE, OUTPUT);
	fastpinMode(SIG_OUT, OUTPUT);
	fastpinMode(RX, OUTPUT);
	fastpinMode(KEY_OUT, OUTPUT);
#ifdef ONEBUTTON
	fastpinMode(BUTTONS, INPUT_PULLUP);  // rotary button
#else
	fastpinMode(BUTTONS, INPUT);  // L/R/rotary button
#endif
	fastpinMode(DIT, INPUT_PULLUP);
	fastpinMode(DAH, INPUT);  // pull-up DAH 10k via AVCC

	fastdigitalWrite(AUDIO1, LOW);  // when used as output, help can mute RX leakage into AREF
	fastdigitalWrite(AUDIO2, LOW);
	fastpinMode(AUDIO1, INPUT);
	fastpinMode(AUDIO2, INPUT);

#ifdef NTX
	fastdigitalWrite(NTX, HIGH);
	fastpinMode(NTX, OUTPUT);
#endif //NTX
#ifdef PTX
	fastdigitalWrite(PTX, LOW);
	fastpinMode(PTX, OUTPUT);
#endif //PTX
#ifdef SWR_METER
	fastpinMode(PIN_FWD, INPUT);
	fastpinMode(PIN_REF, INPUT);
#endif
#ifdef OLED  // assign unused LCD pins
	fastpinMode(PD4, OUTPUT);
	fastpinMode(PD5, OUTPUT);
#else
#if defined(RED_CORNERS) || defined(BLACK_BRICK)
	fastpinMode(PD5, OUTPUT);    // G8RDI mod as drives LCD 1602 backlight
#else
	fastpinMode(PD3, OUTPUT);    // G8RDI mod - uSDX+ have backlight control
#endif
#endif
}

#ifdef CAT
// CAT support inspired by Charlie Morris, ZL2CTM, contribution by Alex, PE1EVX, source: http://zl2ctm.blogspot.com/2020/06/digital-modes-transceiver.html?m=1
// https://www.kenwood.com/i/products/info/amateur/ts_480/pdf/ts_480_pc.pdf
#define CATCMD_SIZE   32
char CATcmd[CATCMD_SIZE];

#ifdef CAT_STREAMING  // G8RDI mod - moved before so in scope
void Command_UA(char en)
{
	char Catbuffer[16];
	sprintf(Catbuffer, "UA%01u;", (_cat_streaming = (en == '1')));
	Serial.print(Catbuffer);
	if (_cat_streaming) { Serial.print("US"); cat_streaming = true; }
}
#endif

void analyseCATcmd()    // Supported Kenwood TS-480 protocol CAT commands
{
	if ((CATcmd[0] == 'F') && (CATcmd[1] == 'A') && (CATcmd[2] == ';'))
		Command_GETFreqA();

	else if ((CATcmd[0] == 'F') && (CATcmd[1] == 'A') && (CATcmd[13] == ';'))
		Command_SETFreqA();

	else if ((CATcmd[0] == 'I') && (CATcmd[1] == 'F') && (CATcmd[2] == ';'))
		Command_IF();

	else if ((CATcmd[0] == 'I') && (CATcmd[1] == 'D') && (CATcmd[2] == ';'))
		Command_ID();

	else if ((CATcmd[0] == 'P') && (CATcmd[1] == 'S') && (CATcmd[2] == ';'))
		Command_PS();

	else if ((CATcmd[0] == 'P') && (CATcmd[1] == 'S') && (CATcmd[2] == '1'))
		Command_PS1();

	else if ((CATcmd[0] == 'A') && (CATcmd[1] == 'I') && (CATcmd[2] == ';'))
		Command_AI();

	else if ((CATcmd[0] == 'A') && (CATcmd[1] == 'I') && (CATcmd[2] == '0'))
		Command_AI0();

	else if ((CATcmd[0] == 'M') && (CATcmd[1] == 'D') && (CATcmd[2] == ';'))
		Command_GetMD();

	else if ((CATcmd[0] == 'M') && (CATcmd[1] == 'D') && (CATcmd[3] == ';'))
		Command_SetMD();

	else if ((CATcmd[0] == 'R') && (CATcmd[1] == 'X') && (CATcmd[2] == ';'))
		Command_RX();

	else if ((CATcmd[0] == 'T') && (CATcmd[1] == 'X') && (CATcmd[2] == ';'))
		Command_TX0();

	else if ((CATcmd[0] == 'T') && (CATcmd[1] == 'X') && (CATcmd[2] == '0'))
		Command_TX0();

	else if ((CATcmd[0] == 'T') && (CATcmd[1] == 'X') && (CATcmd[2] == '1'))
		Command_TX1();

	else if ((CATcmd[0] == 'T') && (CATcmd[1] == 'X') && (CATcmd[2] == '2'))
		Command_TX2();

	else if ((CATcmd[0] == 'A') && (CATcmd[1] == 'G') && (CATcmd[2] == '0'))  // add
		Command_AG0();

	else if ((CATcmd[0] == 'X') && (CATcmd[1] == 'T') && (CATcmd[2] == '1'))  // add
		Command_XT1();

	else if ((CATcmd[0] == 'R') && (CATcmd[1] == 'T') && (CATcmd[2] == '1'))  // add
		Command_RT1();

#ifdef RIT_ENABLE
	else if ((CATcmd[0] == 'R') && (CATcmd[1] == 'T') && (CATcmd[2] == 'S') && (CATcmd[8] == ','))  // GW8RDI mod - set RIT receiver offset
		Command_RTS();	// “RTSxxxxx; ” Sets RIT frequency
#endif

#ifdef CAT_XO_CMD
	else if ((CATcmd[0] == 'X') && (CATcmd[1] == 'O') && (CATcmd[14] == ';'))  // GW8RDI mod - Added/invented : set TIT transmitter offset "XO000000012000;"
		Command_XO();	// -12KHz == "XO100000012000;" Sets TX offset frequency
#endif

	else if ((CATcmd[0] == 'R') && (CATcmd[1] == 'C') && (CATcmd[2] == ';'))  // add
		Command_RC();

	else if ((CATcmd[0] == 'F') && (CATcmd[1] == 'L') && (CATcmd[2] == '0'))  // need?
		Command_FL0();

	else if ((CATcmd[0] == 'R') && (CATcmd[1] == 'S') && (CATcmd[2] == ';'))
		Command_RS();

	else if ((CATcmd[0] == 'V') && (CATcmd[1] == 'X') && (CATcmd[2] != ';'))
		Command_VX(CATcmd[2]);

#ifdef CAT_EXT
	else if ((CATcmd[0] == 'U') && (CATcmd[1] == 'K') && (CATcmd[4] == ';'))  // remote key press
		Command_UK(CATcmd[2], CATcmd[3]);

	else if ((CATcmd[0] == 'U') && (CATcmd[1] == 'D') && (CATcmd[2] == ';'))  // display contents
		Command_UD();
#endif //CAT_EXT

#ifdef CAT_STREAMING
	else if ((CATcmd[0] == 'U') && (CATcmd[1] == 'A') && (CATcmd[3] == ';'))  // audio streaming enable/disable
		Command_UA(CATcmd[2]);
#endif //CAT_STREAMING

	else {
		Serial.print("?;");
#ifdef DEBUG
		{ lcd.setCursor(0, 0); lcd.print(CATcmd); lcd_blanks(); }  // Print error cmd
#endif
	}
}

#ifdef CAT
volatile uint8_t cat_ptr = 0;
void serialEvent() {
	if (Serial.available()) {
		rxend_event = millis() + 10;  // block display until this moment, to prevent CAT cmds that initiate display changes to interfere with the next CAT cmd e.g. Hamlib: FA00007071000;ID;
		char data = Serial.read();
		CATcmd[cat_ptr++] = data;
		if (data == ';') {
			CATcmd[cat_ptr] = '\0'; // terminate the array
			cat_ptr = 0;            // reset for next CAT command
#ifdef _SERIAL
			if (!cat_active) { cat_active = 1; smode = 0; } // disable smeter to reduce display activity
#endif
#ifdef CAT_STREAMING
			if (cat_streaming) { noInterrupts(); cat_streaming = false; Serial.print(';'); }   // terminate CAT stream
			analyseCATcmd();   // process CAT cmd
			if (_cat_streaming) { Serial.print("US"); cat_streaming = true; }  // resume CAT stream
			interrupts();
#else
			analyseCATcmd();
#endif // CAT_STREAMING
			delay(10);
		}
		else if (cat_ptr > (CATCMD_SIZE - 1)) { Serial.print("E;"); cat_ptr = 0; } // overrun
	}
}
#endif //CAT

#ifdef CAT_EXT
void Command_UK(char k1, char k2)
{
	cat_key = ((k1 - '0') << 4) | (k2 - '0');
	if (cat_key & 0x40) { encoder_val--; cat_key &= 0x3f; }
	if (cat_key & 0x80) { encoder_val++; cat_key &= 0x3f; }
	char Catbuffer[16];
	sprintf(Catbuffer, "UK%c%c;", k1, k2);
	Serial.print(Catbuffer);
}

void Command_UD()
{
	char Catbuffer[40];
	sprintf(Catbuffer, "UD%02u%s;", (lcd.curs) ? lcd.y * 16 + lcd.x : 16 * 2 + 1, lcd.text);
	Serial.print(Catbuffer);
}

#endif // CAT_EXT

void Command_GETFreqA()
{
#ifdef _SERIAL
	if (!cat_active) return;
#endif
	char Catbuffer[32];
	unsigned int g, m, k, h;
	uint32_t tf;

	tf = freq;
	g = (unsigned int)(tf / 1000000000lu);
	tf -= g * 1000000000lu;
	m = (unsigned int)(tf / 1000000lu);
	tf -= m * 1000000lu;
	k = (unsigned int)(tf / 1000lu);
	tf -= k * 1000lu;
	h = (unsigned int)tf;

	sprintf(Catbuffer, "FA%02u%03u", g, m);
	Serial.print(Catbuffer);
	sprintf(Catbuffer, "%03u%03u;", k, h);
	Serial.print(Catbuffer);
}

void Command_SETFreqA()
{
	uint32_t fq = (uint32_t)atol(CATcmd + 2);  // "FA00024000000;" GW8RDI mod - CAT freq error check
	if (fq >= 1500000 && fq <= 60000000)   // Ignore corrupted freq data
	{
		freq = fq;
		change = true;
	}
}

void Command_IF()
{
#ifdef _SERIAL
	if (!cat_active) return;
#endif
	char Catbuffer[32];
	unsigned int g, m, k, h;
	uint32_t tf;

	tf = freq;
	g = (unsigned int)(tf / 1000000000lu);
	tf -= g * 1000000000lu;
	m = (unsigned int)(tf / 1000000lu);
	tf -= m * 1000000lu;
	k = (unsigned int)(tf / 1000lu);
	tf -= k * 1000lu;
	h = (unsigned int)tf;

	sprintf(Catbuffer, "IF%02u%03u%03u%03u", g, m, k, h);
	Serial.print(Catbuffer);
	sprintf(Catbuffer, "00000+000000");
	Serial.print(Catbuffer);
	sprintf(Catbuffer, "0000");
	Serial.print(Catbuffer);
	Serial.print(mode + 1);
	sprintf(Catbuffer, "0000000;");
	Serial.print(Catbuffer);
}

void Command_AI()
{
	Serial.print("AI0;");
}

void Command_AG0()
{
	Serial.print("AG0;");
}

void Command_XT1()
{
	Serial.print("XT1;");
}

void Command_RT1()
{
	Serial.print("RT1;");
}

#ifdef CAT_XO_CMD
void Command_XO()		// GW8RDI mod - added set TX offset, i.e. "XO000000012000;"
{
	int32_t fq = atol(CATcmd + 3);  // GW8RDI mod - CAT freq error check
	if (CATcmd[2] == 1)
		fq = -fq;
	{
		tit = fq;
	}
}
#endif

#ifdef RIT_ENABLE
void Command_RTS()		// GW8RDI mod - added set RIT offset, i.e. "RTS30000;"
{
	int32_t fq = atol(CATcmd + 3);  // GW8RDI mod - CAT freq error check
	if (fq >= -99999 && fq <= 99999)   // Ignore corrupted freq data
	{
		rit = fq;
		change = true;
	}
}
#endif

void Command_RC()
{
	rit = 0;
	Serial.print("RC;");
}

void Command_FL0()
{
	Serial.print("FL0;");
}

void Command_GetMD()
{
	Serial.print("MD");
	Serial.print(mode + 1);
	Serial.print(';');
}

void Command_SetMD()
{
  prev_mode = mode;
	mode = CATcmd[2] - '1';
  changedModeCAT = true;
}

void Command_AI0()
{
	Serial.print("AI0;");
}

void Command_RX()
{
#ifdef TX_ENABLE
	switch_rxtx(0);
	semi_qsk_timeout = 0;  // hack: fix for multiple RX cmds
#endif
	Serial.print("RX0;");
}

void Command_TX0()
{
#ifdef TX_ENABLE
	switch_rxtx(1);
#endif
}

void Command_TX1()
{
#ifdef TX_ENABLE
	switch_rxtx(1);
#endif
}

void Command_TX2()
{
#ifdef TX_ENABLE
	switch_rxtx(1);
#endif
}

void Command_RS()
{
	Serial.print("RS0;");
}

void Command_VX(char mode)
{
	char Catbuffer[16];
	sprintf(Catbuffer, "VX%c;", mode);
	Serial.print(Catbuffer);
}

void Command_ID()
{
	Serial.print("ID020;");
}

void Command_PS()
{
	Serial.print("PS1;");
}

void Command_PS1()
{
}
#endif //CAT

void fatal(const __FlashStringHelper * msg, int value = 0, char unit = '\0') {
	lcd.setCursor(0, 1);
	lcd.print('!'); lcd.print('!');
	lcd.print(msg);
	if (unit != '\0') {
		lcd.print('=');
		lcd.print(value);
		lcd.print(unit);
	}
	lcd_blanks();
	delay(1500);
	wdt_reset();
}

//refresh LUT based on pwm_min, pwm_max
void build_lut()
{
	for (uint16_t i = 0; i != 256; i++)    // refresh LUT based on pwm_min, pwm_max
		lut[i] = (i * (pwm_max - pwm_min)) / 255 + pwm_min;
}

#ifdef SWR_METER
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 03/02/2026 - 9M2GRC
// Replace floating point math to fixed-point arithmetic in order to save program memory  
////////////////////////////////////////////////////////////////////////
#if defined(TRUSDX) || defined(RED_CORNERS) || defined(BLACK_BRICK)
void readSWR() {
    uint32_t sum_FWD = 0;
    uint32_t sum_REF = 0;

    // 1- Accumulate raw ADC values
    for (uint8_t i = 0; i < 8; i++) {
        sum_FWD += analogRead(PIN_FWD);
        sum_REF += analogRead(PIN_REF);
        delay(5);
    }
#ifdef TRUSDX
	uint32_t v_FWD_int = (sum_FWD * 4) / 91; 
    uint32_t v_REF_int = (sum_REF * 4) / 111;
#else
#ifdef BLACK_BRICK
    uint32_t v_FWD_int = (sum_FWD * 500) / 6138UL; 
    uint32_t v_REF_int = (sum_REF * 500) / 6138UL;
#else
	uint32_t v_FWD_int = (sum_FWD * 575UL) / 8184UL; 
    uint32_t v_REF_int = (sum_REF * 575UL) / 8184UL;
#endif // BLACK_BRICK 	
#endif // TRUSDX  	
	// Power = V^2. Since V is scaled by 100, V^2 is scaled by 10,000.
    // Divide by 100 to bring it back to a scale of 100 (2 decimal places).
    uint32_t p_FWD_int = (v_FWD_int * v_FWD_int) / 100;
    
    uint32_t VSWR_int = 999;
	// Ensure signal is present
    if (v_FWD_int > 5) { 
        uint32_t vRatio = (v_REF_int * 100) / v_FWD_int;
        if (vRatio < 100) {
            VSWR_int = (100 + vRatio) * 100 / (100 - vRatio);
        }
    }
    if (VSWR_int > 999 || VSWR_int < 100) VSWR_int = 999;

    // 3. Update Display
    if (p_FWD_int != FWD || VSWR_int != SWR) {
        lcd.noCursor();
        lcd.setCursor(0, 0);
        
        switch (swrmeter) {
            case 1:
                lcd.print(" "); printFixed(p_FWD_int); 
                lcd.print(F("W SWR:")); printFixed(VSWR_int);
                break;
            case 2:
                uint32_t p_REV_int = (v_REF_int * v_REF_int) / 100;
                lcd.print(F(" F:")); printFixed(p_FWD_int); 
                lcd.print(F("W R:")); printFixed(p_REV_int); lcd.print("W");
                break;
            case 3:
                lcd.print(F(" F:")); printFixed(v_FWD_int); 
                lcd.print(F("V R:")); printFixed(v_REF_int); lcd.print("V");
                break;
        }
		
		FWD = p_FWD_int;
        SWR = VSWR_int;
    }
}
// Helper function to print "X.XX" format using integers
void printFixed(uint32_t val) {
    lcd.print(val / 100);
    lcd.print(".");
    if ((val % 100) < 10) lcd.print("0");
    lcd.print(val % 100);
}
#else
void readSWR()
// reads FWD / REF values from A6 and A7 and computes SWR
{
	float v_FWD = 0;
	float v_REF = 0;
	for (int i = 0; i <= 7; i++) {
		v_FWD = v_FWD + (ref_V / 1023) * (int)analogRead(PIN_FWD);
		v_REF = v_REF + (ref_V / 1023) * (int)analogRead(PIN_REF);
		delay(5);
	}
////////////////////////////////////////////////////////////////////////
// Adding (tr)uSDX option for CLASSIC, LO and HI filter bands for (tr)uSDX
// LO Bands      - 20/30/40/60/80 meter bands
// CLASSIC Bands - 10/15/20/40/80 meter bands
// HI Bands      - 10/12/15/17/20 meter bands  
// 18/01/2026 - 9M2GRC
// Adding (tr)uSDX forward and reverse new calculation based on experimentation
////////////////////////////////////////////////////////////////////////
#ifdef TRUSDX
	v_FWD = (v_FWD / 8) / 1.39;
	v_REF = (v_REF / 8) / 1.70;
#else 
#ifdef BLACK_BRICK
	v_FWD = v_FWD / 6;	// Mentioned by Ovidiu Băluță that Blackbrick needs a high level
	v_REF = v_REF / 6;
#else
	v_FWD = v_FWD / 8;
	v_REF = v_REF / 8;
#endif
#endif // TRUSDX

	float p_FWD = sq(v_FWD);
	float p_REV = sq(v_REF);

	float vRatio = v_REF / v_FWD;
	float VSWR = (1 + vRatio) / (1 - vRatio);

	if ((VSWR > 9.99) || (VSWR < 1))VSWR = 9.99;

	if (p_FWD != FWD || VSWR != SWR) {
		lcd.noCursor();
		lcd.setCursor(0, 0);
		
		switch (swrmeter) {
		case 1:
			lcd.print(" "); lcd.print(floor(100 * p_FWD) / 100); lcd.print("W  SWR:"); lcd.print(floor(100 * VSWR) / 100);
			break;
		
		case 2:
			lcd.print(" F:"); lcd.print(floor(100 * p_FWD) / 100); lcd.print("W R:"); lcd.print(floor(100 * p_REV) / 100); lcd.print("W");
			break;
		case 3:
			lcd.print(" F:"); lcd.print(floor(100 * v_FWD) / 100); lcd.print("V R:"); lcd.print(floor(100 * v_REF) / 100); lcd.print("V");
			break;
		}
		FWD = p_FWD;
		SWR = VSWR;
	}
}
#endif // TRUSDX
#endif // SWR_METER

void setup()
{
	fastdigitalWrite(KEY_OUT, LOW);  // for safety: to prevent exploding PA MOSFETs, in case there was something still biasing them.
	si5351.powerDown();  // disable all CLK outputs (especially needed for si5351 variants that has CLK2 enabled by default, such as Si5351A-B04486-GT)

	MCUSR = 0;
	wdt_enable(WDTO_4S);  // Enable watchdog
	uint32_t t0, t1;
#ifdef DEBUG
	// Benchmark dsp_tx() ISR (this needs to be done in beginning of setup() otherwise when VERSION containts 5 chars, mis-alignment impact performance by a few percent)
	func_ptr = dsp_tx;
	t0 = micros();
	TIMER2_COMPA_vect();
	t1 = micros();
	uint16_t load_tx = (float)(t1 - t0) * (float)F_SAMP_TX * 100.0 / 1000000.0 * 16000000.0 / (float)F_CPU;
	// benchmark sdr_rx_00() ISR
	func_ptr = sdr_rx_00;
	rx_state = 0;
	uint16_t load_rx[8];
	uint16_t load_rx_avg = 0;
	uint16_t i;
	for (i = 0; i != 8; i++) {
		rx_state = i;
		t0 = micros();
		TIMER2_COMPA_vect();
		t1 = micros();
		load_rx[i] = (float)(t1 - t0) * (float)F_SAMP_RX * 100.0 / 1000000.0 * 16000000.0 / (float)F_CPU;
		load_rx_avg += load_rx[i];
	}
	load_rx_avg /= 8;
#endif //DEBUG
	ADMUX = (1 << REFS0);  // restore reference voltage AREF (5V)

	// disable external interrupts
	PCICR = 0;
	PCMSK0 = 0;
	PCMSK1 = 0;
	PCMSK2 = 0;

	encoder_setup();

	initPins();

	delay(100);           // at least 40ms after power rises above 2.7V before sending commands
	lcd.begin(16, 4);     // Init LCD
#ifndef OLED
	for (uint8_t i = 0; i != N_FONTS; i++) {  // Init fonts
		pgm_cache_item(fonts[i], 8);
		lcd.createChar(0x01 + i, _item);
	}
#endif

	show_banner();
	lcd.setCursor(7, 0); lcd.print(F(" R")); lcd.print(F(VERSION)); lcd_blanks();
	delay(500);	// GW8RDI MOD - SHOW LONGER

#ifdef QCX
	// Test if QCX has DSP/SDR capability: SIDETONE output disconnected from AUDIO2
	si5351.SendRegister(SI_CLK_OE, TX0RX0); // Mute QSD
	fastdigitalWrite(RX, HIGH);  // generate pulse on SIDETONE and test if it can be seen on AUDIO2
	delay(1); // settle
	fastdigitalWrite(SIDETONE, LOW);
	int16_t v1 = analogRead(AUDIO2);
	fastdigitalWrite(SIDETONE, HIGH);
	int16_t v2 = analogRead(AUDIO2);
	fastdigitalWrite(SIDETONE, LOW);
	dsp_cap = !(abs(v2 - v1) > (0.05 * 1024.0 / 5.0));  // DSP capability?
	if (dsp_cap) {  // Test if QCX has SDR capability: AUDIO2 is disconnected from AUDIO1  (only in case of DSP capability)
		delay(400); wdt_reset(); // settle:  the following test only works well 400ms after startup
		v1 = analogRead(AUDIO1);
		fastdigitalWrite(AUDIO2, HIGH);   // generate pulse on AUDIO2 and test if it can be seen on AUDIO1
		fastpinMode(AUDIO2, OUTPUT);
		delay(1);
		fastdigitalWrite(AUDIO2, LOW);
		delay(1);
		fastdigitalWrite(AUDIO2, HIGH);
		v2 = analogRead(AUDIO1);
		fastpinMode(AUDIO2, INPUT);
		if (!(abs(v2 - v1) > (0.125 * 1024.0 / 5.0))) dsp_cap = SDR;  // SDR capacility?
	}
	// Test if QCX has SSB capability: DAH is connected to DVM
	delay(1); // settle
	fastpinMode(DAH, OUTPUT);
	fastdigitalWrite(DAH, LOW);
	v1 = analogRead(DVM);
	fastdigitalWrite(DAH, HIGH);
	v2 = analogRead(DVM);
	fastdigitalWrite(DAH, LOW);
	fastpinMode(DAH, INPUT);
	ssb_cap = (abs(v2 - v1) > (0.05 * 1024.0 / 5.0));  // SSB capability?
#endif // QCX

#ifdef DEBUG
  // Measure CPU loads
	if (!(load_tx <= 100)) {
		fatal(F("CPU_tx"), load_tx, '%');
	}

	if (!(load_rx_avg <= 100)) {
		fatal(F("CPU_rx"), load_rx_avg, '%');
	}
#endif

#ifdef DIAG
	// Measure VDD (+5V); should be ~5V
	si5351.SendRegister(SI_CLK_OE, TX0RX0); // Mute QSD
	fastdigitalWrite(KEY_OUT, LOW);
	fastdigitalWrite(RX, LOW);  // mute RX
	delay(100); // settle
	float vdd = 2.0 * (float)analogRead(AUDIO2) * 5.0 / 1024.0;
	fastdigitalWrite(RX, HIGH);
	if (!(vdd > 4.8 && vdd < 5.2)) {
		fatal(F("V5.0"), vdd, 'V');
	}

	// Measure VEE (+3.3V); should be ~3.3V
	float vee = (float)analogRead(SCL) * 5.0 / 1024.0;
	if (!(vee > 3.2 && vee < 3.8)) {
		fatal(F("V3.3"), vee, 'V');
	}

	// Measure AVCC via AREF and using internal 1.1V reference fed to ADC; should be ~5V
	analogRead(6); // setup almost proper ADC readout
	bitSet(ADMUX, 3); // Switch to channel 14 (Vbg=1.1V)
	delay(1); // delay improves accuracy
	bitSet(ADCSRA, ADSC);
	for (; bit_is_set(ADCSRA, ADSC););
	float avcc = 1.1 * 1023.0 / ADC;
	if (!(avcc > 4.6 && avcc < 5.2)) {
		fatal(F("Vavcc"), avcc, 'V');
	}

	// Report no SSB capability
	if (!ssb_cap) {
		fatal(F("No MIC input..."));
	}

	// Measure DVM bias; should be ~VAREF/2
#ifdef _SERIAL
	DDRC &= ~(1 << 2);  // disable PC2, so that ADC2 can be used as mic input
#else
	PORTD |= 1 << 1; DDRD |= 1 << 1;  // keep PD1 HIGH so that in case diode is installed to PC2 it is kept blocked (otherwise ADC2 input is pulled down!)
#endif
	delay(10);
#ifdef TX_ENABLE
	float dvm = (float)analogRead(DVM) * 5.0 / 1024.0;
	if ((ssb_cap) && !(dvm > 1.8 && dvm < 3.2)) {
		fatal(F("Vadc2"), dvm, 'V');
	}
#endif

	// Measure AUDIO1, AUDIO2 bias; should be ~VAREF/2
	if (dsp_cap == SDR) {
		float audio1 = (float)analogRead(AUDIO1) * 5.0 / 1024.0;
		if (!(audio1 > 1.8 && audio1 < 3.2)) {
			fatal(F("Vadc0"), audio1, 'V');
		}
		float audio2 = (float)analogRead(AUDIO2) * 5.0 / 1024.0;
		if (!(audio2 > 1.8 && audio2 < 3.2)) {
			fatal(F("Vadc1"), audio2, 'V');
		}
	}

#ifdef TX_ENABLE
	// Measure I2C Bus speed for Bulk Transfers
	wdt_reset();
	t0 = micros();
	for (uint16_t i = 0; i != 1000; i++) si5351.SendPLLRegisterBulk();
	t1 = micros();
	uint32_t speed = (1000000 * 8 * 7) / (t1 - t0); // speed in kbit/s
	if (false) {
		fatal(F("i2cspeed"), speed, 'k');
	}

	// Measure I2C Bit-Error Rate (BER); should be error free for a thousand random bulk PLLB writes
	si5351.freq(freq, 0, 90);  // freq needs to be set in order to use freq_calc_fast()
	wdt_reset();
	uint16_t i2c_error = 0;  // number of I2C byte transfer errors
	for (uint16_t i = 0; i != 1000; i++) {
		si5351.freq_calc_fast(i);
		si5351.SendPLLRegisterBulk();
#define SI_SYNTH_PLL_A 26
#ifdef NEW_TX
		for (int j = 4; j != 8; j++) if (si5351.RecvRegister(SI_SYNTH_PLL_A + j) != si5351.pll_regs[j]) i2c_error++;
#else
		for (int j = 3; j != 8; j++) if (si5351.RecvRegister(SI_SYNTH_PLL_A + j) != si5351.pll_regs[j]) i2c_error++;
#endif //NEW_TX
	}
	wdt_reset();
	if (i2c_error) {
		fatal(F("BER_i2c"), i2c_error, ' ');
	}
#endif //TX_ENABLE
#endif  // DIAG

	drive = 4;  // Init settings
#ifdef QCX
	if (!ssb_cap) { vfomode[0] = CW; vfomode[1] = CW; filt = 4; stepsize = STEP_500; }
	if (dsp_cap != SDR) pwm_max = 255; // implies that key-shaping circuit is probably present, so use full-scale
	if (dsp_cap == DSP) volume = 10;
	if (!dsp_cap) cw_tone = 2;   // use internal 700Hz QCX filter, so use same offset and keyer tone
#endif //QCX
	cw_offset = tones[cw_tone];

#ifdef KEEP_BAND_DATA  // G8RDI mod
	memset(freq_last, 0, sizeof(freq_last));   // G8RDI mod - set to default
	mode_last[0] = mode_last[1] = mode_last[2] = mode_last[3] = LSB;
	mode_last[4] = mode_last[5] = mode_last[6] = mode_last[7] = mode_last[8] = USB;   // Set for up to 9 bands only xyzzy
#endif

	// Load parameters from EEPROM, reset to factory defaults when stored values are from a different version
	paramAction(LOAD, VERS);

	if ((eeprom_version != get_version_id()) || _digitalRead(BUTTONS)) {  // EEPROM clean: if rotary-key pressed or version signature in EEPROM does NOT corresponds with this firmware
		eeprom_version = get_version_id();

		paramAction(SAVE);  // save default parameter values
		lcd.setCursor(0, 1); lcd.print(F("Reset settings.."));
		delay(500); wdt_reset();
	}
	else {
		paramAction(LOAD);  // load all parameters
	}
	si5351.iqmsa = 0;  // enforce PLL reset
	change = true;
	prev_bandval = bandval;
	vox = false;  // disable VOX
	rit = false;  // disable RIT
	freq = vfo[vfosel % 2];
	mode = vfomode[vfosel % 2];

#ifdef NR_FIR
	if (nr > 2)
		FirFilterSetup(7 + (((nr - 2) - 1) * 2), filt_val[filt], F_SAMP_RX / 8);  // GW8RDI mod
#endif

#ifdef TX_ENABLE
	build_lut();
#endif

	delay(800); // G8RDI mod added so visible

	show_banner();  // remove release number

	start_rx();   // Start radio receiver

#if defined(CAT) || defined(TESTBENCH)
#ifdef CAT_STREAMING
#define BAUD   115200           //  Baudrate used for serial communications
#else
#ifdef CAT_FAST
#define BAUD   115200           // 57600/115200 Fast comms, see if reduces audio glitching on CAT polling G8RDI mod
#else
#define BAUD   38400            // 38400 57600//115200//4800 //Baudrate used for serial communications (CAT, TESTBENCH)
#endif
#endif

	if (cat_enabled)  // G8RDI mod
	{
		Serial.begin(16000000ULL * BAUD / F_MCU); // corrected for F_CPU=20M
		Command_IF();
#if !defined(OLED) && defined(TESTBENCH)
		smode = 0;  // In case of LCD, turn off smeter
#endif
	}
#endif //CAT TESTBENCH

#ifdef KEYER
	keyerState = IDLE;
	keyerControl = IAMBICB;      // Or 0 for IAMBICA
	loadWPM(keyer_speed);        // Fix speed at 15 WPM
#endif //KEYER

	for (; !_digitalRead(DIT) || ((mode == CW && keyer_mode != SINGLE) && (!_digitalRead(DAH)));) { fatal(F("Check PTT/key")); }// wait until DIH/DAH/PTT is released to prevent TX on startup
}

static int32_t _step = 0;

void loop()
{
#ifdef VOX_ENABLE
	if ((vox) && ((mode == LSB) || (mode == USB))) {  // If VOX enabled (and in LSB/USB mode), then take mic samples and feed ssb processing function, to derive amplitude, and potentially detect cross vox_threshold to detect a TX or RX event: this is expressed in tx variable
		if (!vox_tx) { // VOX not active
#ifdef MULTI_ADC
			if (vox_sample++ == 16) {  // take N sample, then process
				ssb(((int16_t)(vox_adc / 16) - (512 - AF_BIAS)) >> MIC_ATTEN);   // sampling mic
				vox_sample = 0;
				vox_adc = 0;
			}
			else {
				vox_adc += analogSampleMic();
			}
#else
			ssb(((int16_t)(analogSampleMic()) - 512) >> MIC_ATTEN);   // sampling mic
#endif
			if (tx) {  // TX triggered by audio -> TX
				vox_tx = 1;
				switch_rxtx(255);
			}
		}
		else if (!tx) {  // VOX activated, no audio detected -> RX
			switch_rxtx(0);
			vox_tx = 0;
			delay(32); //delay(10);
		}
	}
#endif //VOX_ENABLE

#ifdef CW_DECODER
	if ((mode == CW) && cwdec && ((!tx) && (!semi_qsk_timeout))) cw_decode();  // CW decoder only active during RX
#endif  //CW_DECODER

	if (menumode == 0) { // in main
#ifdef CW_DECODER
		if (cw_event) {
			const char offv[] = { 0, 7, 3, 5, 3, 7, 8 };    // G8RDI mod - adjusted to meet standard
			uint8_t offset = offv[smode]; // depending on smeter more/less cw-text
			lcd.noCursor();
#ifdef OLED
			uint8_t i = cw_event - 1; if (15 - offset - i + 1) { lcd.setCursor(15 - offset - i, 0); lcd.print(out[15 - i]); cw_event++; }
			else cw_event = false;  // since an oled string write would hold-up reliable decoding/keying, write only a single char each time and continue
#else
			cw_event = false;
			lcd.setCursor(0, 0); lcd.print(out + offset);
#endif
			stepsize_showcursor();
		}
		else
#endif  //CW_DECODER
			if ((!semi_qsk_timeout) && (!vox_tx))
				smeter();
	}

#ifdef KEYER  //Keyer
	if (mode == CW && keyer_mode != SINGLE)  // check DIT/DAH keys for CW
  {
		switch (keyerState) { // Basic Iambic Keyer, keyerControl contains processing flags and keyer mode bits, Supports Iambic A and B, State machine based, uses calls to millis() for timing.
		case IDLE: // Wait for direct or latched paddle press
			if ((_digitalRead(DAH) == LOW) ||
				(_digitalRead(DIT) == LOW) ||
				(keyerControl & 0x03))
			{
#ifdef CW_MESSAGE
				cw_msg_event = 0;  // clear cw message event
#endif //CW_MESSAGE
				update_PaddleLatch();
				keyerState = CHK_DIT;
			}
			break;
		case CHK_DIT: // See if the dit paddle was pressed
			if (keyerControl & DIT_L) {
				keyerControl |= DIT_PROC;
				ktimer = ditTime;
				keyerState = KEYED_PREP;
			}
			else {
				keyerState = CHK_DAH;
			}
			break;
		case CHK_DAH: // See if dah paddle was pressed
			if (keyerControl & DAH_L) {
				ktimer = ditTime * 3;
				keyerState = KEYED_PREP;
			}
			else {
				keyerState = IDLE;
			}
			break;
		case KEYED_PREP: // Assert key down, start timing, state shared for dit or dah
			Key_state = HIGH;
			switch_rxtx(Key_state);
			ktimer += millis();                 // set ktimer to interval end time
			keyerControl &= ~(DIT_L + DAH_L);   // clear both paddle latch bits
			keyerState = KEYED;                 // next state
			break;
		case KEYED: // Wait for timer to expire
			if (millis() > ktimer) {            // are we at end of key down ?
				Key_state = LOW;
				switch_rxtx(Key_state);
				ktimer = millis() + ditTime;    // inter-element time
				keyerState = INTER_ELEMENT;     // next state
			}
			else if (keyerControl & IAMBICB) {
				update_PaddleLatch();           // early paddle latch in Iambic B mode
			}
			break;
		case INTER_ELEMENT:
			// Insert time between dits/dahs
			update_PaddleLatch();               // latch paddle state
			if (millis() > ktimer) {            // are we at end of inter-space ?
				if (keyerControl & DIT_PROC) {             // was it a dit or dah ?
					keyerControl &= ~(DIT_L + DIT_PROC);   // clear two bits
					keyerState = CHK_DAH;                  // dit done, check for dah
				}
				else {
					keyerControl &= ~(DAH_L);              // clear dah latch
					keyerState = IDLE;                     // go idle
				}
			}
			break;
		}

	}
	else {
#endif //KEYER

#ifdef TX_ENABLE
		uint8_t pin = ((mode == CW) && (keyer_swap)) ? DAH : DIT;
		if (!vox_tx)  //  ONLY if VOX not active, then check DIT/DAH (fix for VOX to prevent RFI feedback through EMI on DIT or DAH line)
			if (!_digitalRead(pin)) {  // PTT/DIT keys transmitter
#ifdef CW_MESSAGE
				cw_msg_event = 0;  // clear cw message event
#endif //CW_MESSAGE
				switch_rxtx(1);   // Switch to TX mode
				do {
					wdt_reset();
					delay((mode == CW) ? 10 : 100);  // keep the tx keyed for a while before sensing (helps against RFI issues on DAH/DAH line)
#ifdef SWR_METER
					if (smeter > 0 && mode == CW && millis() >= stimer) { readSWR(); stimer = millis() + 500; }
#endif
					if (inv ^ _digitalRead(BUTTONS)) break;  // break if button is pressed (to prevent potential lock-up)
				} while (!_digitalRead(pin)); // until released
				switch_rxtx(0);
			}
#endif //TX_ENABLE
#ifdef KEYER
	}
#endif //KEYER

#ifdef SEMI_QSK
	if ((semi_qsk_timeout) && (millis() > semi_qsk_timeout)) { switch_rxtx(0); }  // delayed QSK RX
#endif

	enum event_t { BL = 0x10, BR = 0x20, BE = 0x30, SC = 0x01, DC = 0x02, PL = 0x04, PLC = 0x05, PT = 0x0C }; // button-left, button-right and button-encoder; single-click, double-click, push-long, push-and-turn

	if (inv ^ _digitalRead(BUTTONS))   // Left-/Right-/Rotary-button (while not already pressed)
  {
		if (!((event & PL) || (event & PLC))) {  // hack: if there was long-push before, then fast forward
			uint16_t v = analogSafeRead(BUTTONS);
#ifdef CAT_EXT
			if (cat_key) { v = (cat_key & 0x04) ? 512 : (cat_key & 0x01) ? 870 : (cat_key & 0x02) ? 1024 : 0; }  // override analog value exercised by BUTTONS press
#endif //CAT_EXT
			event = SC;
			int32_t t0 = millis();
			for (; inv ^ _digitalRead(BUTTONS);) { // until released or long-press
				if ((millis() - t0) > 300) { event = PL; break; }
				wdt_reset();
			}
			delay(10); //debounce
			for (; (event != PL) && ((millis() - t0) < 500);) { // until 2nd press or timeout
				if (inv ^ _digitalRead(BUTTONS)) { event = DC; break; }
				wdt_reset();
			}
			for (; inv ^ _digitalRead(BUTTONS);) { // until released, or encoder is turned while longpress
				if (encoder_val && event == PL) { event = PT; break; }
#ifdef ONEBUTTON
				if (event == PL) break;  // do not lock on longpress, so that L and R buttons can be used for tuning
#endif
				wdt_reset();
			}
#ifndef TRUSDX	
			// Max. voltages at ADC3 for buttons L,R,E: 3.76V;4.55V;5V, thresholds are in center
			event |= (v < (uint16_t)(4.2 * 1024.0 / 5.0)) ? BL : (v < (uint16_t)(4.8 * 1024.0 / 5.0)) ? BR : BE; // determine which button pressed based on threshold levels
#else
			event |= (v < (uint16_t)(3.76 * 1024.0 / 5.0)) ? BL : (v < (uint16_t)(4.75 * 1024.0 / 5.0)) ? BR : BE; // determine which button pressed based on threshold levels
#endif // TRUSDX
		}
		else {  // hack: fast forward handling
			event = (event & 0xf0) | ((encoder_val) ? PT : PLC/*PL*/);  // only alternate between push-long/turn when applicable
		}

		switch (event) {
#ifndef ONEBUTTON
		case BL | PL:  // Called when menu button pressed
		case BL | PLC: // or kept pressed
			menumode = 2;
			break;
		case BL | PT:
			menumode = 1;
			break;
		case BL | SC:
#ifdef CW_MESSAGE
			if ((menumode == 1) && (menu >= CWMSG1) && (menu <= CWMSG6)) {
				cw_msg_event = millis();
				cw_msg_id = menu - CWMSG1;
				menumode = 0;
				break;
			}
#endif //CW_MESSAGE
			int8_t _menumode;
			if (menumode == 0) { _menumode = 1; if (menu == 0) menu = 1; }  // short left-click while in default screen: enter menu mode
			if (menumode == 1) { _menumode = 2; }                          // short left-click while in menu: enter value selection screen
			if (menumode >= 2) { _menumode = 0; paramAction(SAVE, menu); } // short left-click while in value selection screen: save, and return to default screen
			menumode = _menumode;
			break;
		case BL | DC:
			break;
		case BR | SC:   // Mode change - Button Right, Single Click
      changedMode = true; // GW8RDI 230401
			break;
		case BR | DC:
#ifdef NCHGFILTDC
			changedMode = true;
#else 
			filt++;
			_init = true;
			if (mode == CW && filt > N_FILT) filt = 4;
			if (mode == CW && filt == 4) stepsize = STEP_500; // reset stepsize for 500Hz filter
			if (mode == CW && (filt == 5 || filt == 6) && stepsize < STEP_100) stepsize = STEP_100; // for CW BW 200, 100      -> step = 100 Hz
			if (mode == CW && filt == 7 && stepsize < STEP_10) stepsize = STEP_10;                  // for CW BW 50 -> step = 10 Hz
			if (mode != CW && filt > 3) filt = 0;
			encoder_val = 0;
			paramAction(UPDATE, FILTER);
			paramAction(SAVE, FILTER);
			wdt_reset(); delay(1500); wdt_reset();
			change = true; // refresh display
#endif // NCHGFILTDC
#ifdef NR_FIR
			if (nr > 2)
				FirFilterSetup(7 + (((nr - 2) - 1) * 2), filt_val[filt], F_SAMP_RX / 8);  // GW8RDI mod
#endif
			break;
		case BR | PL:
#ifdef SIMPLE_RX
			// Experiment: ISR-less sdr_rx():
			smode = 0;
			TIMSK2 &= ~(1 << OCIE2A);  // disable timer compare interrupt
			delay(100);
			lcd.setCursor(15, 1); lcd.print('X');
			static uint8_t x = 0;
			uint32_t next = 0;
			for (;;) {
				func_ptr();
#ifdef DEBUG
				numSamples++;
#endif
				if (!rx_state) {
					x++;
					if (x > 16) {
						loop();
						x = 0;
					}
				}
			} //
#endif //SIMPLE_RX
#ifdef RIT_ENABLE
			rit = !rit;
			stepsize = (rit) ? STEP_10 : prev_stepsize[mode == CW];
			if (!rit) {  // after RIT comes VFO A/B swap
#else
			{
#endif //RIT_ENABLE
        vfosel = !vfosel;
        freq = vfo[vfosel % 2];  // todo: share code with menumode
        mode = vfomode[vfosel % 2];
        if (mode != CW) stepsize = STEP_1k; else stepsize = STEP_500;
        if (mode == CW) { filt = 4; nr = 0; }
        else filt = 0;
			}
				change = true;
				break;
#ifdef TUNING_DIAL
		case BR | PLC:  // while pressed long continues
		case BE | PLC:
			freq = freq + ((_step > 0) ? 1 : -1) * pow(2, abs(_step)); change = true;
			break;
		case BR | PT:
			_step += encoder_val; encoder_val = 0;
			lcd.setCursor(0, 0); lcd.print(_step); lcd_blanks();
			break;
#endif //TUNING_DIAL
		case BE | SC:
			if (!menumode) {
				stepsize_change(+1);
			}
			else {
				int8_t _menumode;
				if (menumode == 1) { _menumode = 2; }  // short encoder-click while in menu: enter value selection screen
				if (menumode == 2) { _menumode = 1; change = true; paramAction(SAVE, menu); } // short encoder-click while in value selection screen: save, and return to menu screen
#ifdef MENU_STR
				if (menumode == 3) { _menumode = 3; paramAction(NEXT_CH, menu); } // short encoder-click while in string edit mode: change position to next character
#endif
				menumode = _menumode;
			}
			break;

		case BE | DC:     // Button Encoder and DC double-click for Band change (( G8RDI mod - now bi-directional and restores freq & mode)

#ifdef KEEP_BAND_DATA  // G8RDI mod
			prev_bandval = bandval;
			prev_mode = mode;
			if (bandval > 0 && bandval <= BANDCOUNT)   // bandval 1-5/8 (0 is 6m, 9 is 160m)
			{
				freq_last[bandval - 1] = freq;  //vfo[vfosel % 2]    // G8RDI mod - Save freq and mode last used on this band
				mode_last[bandval - 1] = vfomode[vfosel % 2];
			}
#ifdef DEBUG_G8RDI
			else
			{
				error_code = 2;
				show_banner();  // Debug only to show error
				delay(600);
			}
#endif
#endif
#if defined(RED_BUTTONS) || defined(WHITE_BUTTONS) || defined(BLACK_BRICK)  // For some reason, even without SWAP_ROTARY, Red Buttons is reversed
#if defined(REVERSE_BAND_CHANGE)
			if (last_state == 0x13 || last_state == 0x32 || last_state == 0x20 || last_state == 0x01) // --Dir last freq step - G8RDI mod
				bandval++;
			else
				bandval--;
#else
			if (last_state == 0x13 || last_state == 0x32 || last_state == 0x20 || last_state == 0x01) // --Dir last freq step - G8RDI mod
				bandval--;
			else
				bandval++;    //  G8RDI mod to make last freq change control and change dir
#endif  
#else
#if defined(SWAP_ROTARY) || defined(REVERSE_BAND_CHANGE)  // G8RDI mod A7. If your freq. change is correct, but band jump goes backwards, define REVERSE_BAND_CHANGE
			if (last_state == 0x13 || last_state == 0x32 || last_state == 0x20 || last_state == 0x01) // --Dir last freq step - G8RDI mod
				bandval--;
			else
				bandval++;    //  G8RDI mod to make last freq change control and change dir
#else
			if (last_state == 0x13 || last_state == 0x32 || last_state == 0x20 || last_state == 0x01) // --Dir last freq step - G8RDI mod
				bandval++;
			else
				bandval--;    //  G8RDI mod to make last freq change control and change dir
#endif
#endif          
#ifdef TRUSDX
			if (bandval >= (N_BANDS - 5)) 
				bandval = 1;
#else
			if (bandval >= (N_BANDS - 1)) 
				bandval = 1;  // excludes 6m
#endif // TRUSDX
			else
#ifdef TRUSDX
				if (bandval < 1) bandval = N_BANDS - 6;
#else	
				if (bandval < 1) bandval = N_BANDS - 2;  // excludes 160m  // G8RDI mod - added
#endif // TRUSDX

			stepsize = STEP_500;  // G8RDI mod //STEP_1k;
			change = true;
			break;

		case BE | PL: stepsize_change(-1); break;
		case BE | PT:
			for (; _digitalRead(BUTTONS);) { // process encoder changes until released
				wdt_reset();
				if (encoder_val) {
					paramAction(UPDATE, VOLUME);
					if (volume < 0) { volume = 10; paramAction(SAVE, VOLUME); powerDown(); }  // powerDown when volume < 0
					paramAction(SAVE, VOLUME);
				}
			}
			change = true; // refresh display
			break;
#else //ONEBUTTON
		case BE | SC:
			int8_t _menumode;
			if (menumode == 0) { _menumode = 1; if (menu == 0) menu = 1; }  // short enc-click while in default screen: enter menu mode
			if (menumode == 1) { _menumode = 2; }                          // short enc-click while in menu: enter value selection screen
			if (menumode == 2) { _menumode = 0; paramAction(SAVE, menu); } // short enc-click while in value selection screen: save, and return to default screen
#ifdef MENU_STR
			if (menumode == 3) { _menumode = 3; paramAction(NEXT_CH, menu); } // short encoder-click while in string edit mode: change position to next character
#endif
			menumode = _menumode;
			break;
		case BE | PL:
			stepsize += 1;
			if (stepsize < STEP_1k) stepsize = STEP_10;
			if (stepsize > STEP_10) stepsize = STEP_1k;
			stepsize_showcursor();
			break;
		case BE | PLC: // or kept pressed
			menumode = 2;
			break;
		case BE | PT:
			menumode = 1;
			break;
		case BL | SC:
		case BL | DC:
		case BL | PL:
		case BL | PLC:
			encoder_val++;
			break;
		case BR | SC:
		case BR | DC:
		case BR | PL:
		case BR | PLC:
			encoder_val--;
			break;
#endif //ONEBUTTON
			}
		}
	else event = 0;  // no button pressed: reset event

  if (changedMode || changedModeCAT)  // 230401 GW8RDI
  {
    changedMode = false;
  
    if (!menumode)
    {
        if (!changedModeCAT)  // Note: CAT cmd reception sets mode
        {
          prev_mode = mode;
          mode++;  // Change to next mode += 1

#ifdef SHOW_USB_LSB_CW_ONLY
          if (mode > CW)  // Mode button only cycles USB, LSB, CW only
            mode = LSB;   // Skip all other modes (only LSB (0), USB, CW(2))
#else
          if (mode > AM)  // G8RDI mod - *changed from > CW so that all modes can be accessed
            mode = LSB;   // *now shows all / skip all other modes (only LSB (0), USB, CW(2))
#endif
        }
        else
        {
          if (mode > AM)  // G8RDI mod - allow changedModeCAT to change to any mode including AM
            mode = LSB;
        }

#ifdef MODE_CHANGE_RESETS
        if (mode != CW)
        {
          stepsize = STEP_500; // sets suitable stepsize //STEP_1k
          filt = 0;  // resets filter (to most BW) and NR on mode change
        }
        else
        { filt = 4; nr = 0; }
#else
        if (mode == CW) { nr = 0; }
        prev_stepsize[prev_mode == CW] = stepsize; stepsize = prev_stepsize[mode == CW]; // backup stepsize setting for previous mode, restore previous stepsize setting for current selected mode; filter settings captured for either CQ or other modes.
        prev_filt[prev_mode == CW] = filt;
        filt = prev_filt[mode == CW];  // backup filter setting for previous mode, restore previous filter setting for current selected mode; filter settings captured for either CQ or other modes.
#endif
        vfomode[vfosel % 2] = mode;
        paramAction(SAVE, (vfosel % 2) ? MODEB : MODEA);  // save vfoa/b changes
        paramAction(SAVE, MODE);
        paramAction(SAVE, FILTER);
        si5351.iqmsa = 0;  // enforce PLL reset
#ifdef CW_DECODER
        if (prev_mode == CW && cwdec)
          show_banner();
#endif
        change = true;
    }
    else
    {
      if (menumode == 1) { menumode = 0; }  // short right-click while in menu: enter value selection screen
      if (menumode >= 2) { menumode = 1; change = true; paramAction(SAVE, menu); } // short right-click while in value selection screen: save, and return to menu screen
    }
    changedModeCAT = false;
  }

	if ((menumode) || (prev_menumode != menumode)) {  // Show parameter and value
		int8_t encoder_change = encoder_val;
		if ((menumode == 1) && encoder_change) {
			menu += encoder_val;   // Navigate through menu
#ifdef ONEBUTTON
			menu = max(0, min(menu, N_PARAMS));
#else
			if (menu > N_PARAMS)
				menu = 1;
			else
				if (menu < 1)
					menu = N_PARAMS;
#endif
			menu = paramAction(NEXT_MENU, menu);  // auto probe next menu item (gaps may exist)
			encoder_val = 0;
		}
		if (encoder_change || (prev_menumode != menumode)) paramAction(UPDATE_MENU, (menumode) ? menu : 0);  // update param with encoder change and display
		prev_menumode = menumode;
		if (menumode == 2) {
			if (encoder_change) {
				lcd.setCursor(0, 1); lcd.cursor();  // edits menu item value; make cursor visible
				if (menu == MODE) { // post-handling Mode parameter
					vfomode[vfosel % 2] = mode;
					paramAction(SAVE, (vfosel % 2) ? MODEB : MODEA);  // save vfoa/b changes
					change = true;
					si5351.iqmsa = 0;  // enforce PLL reset
					if (mode != CW) stepsize = STEP_1k; else stepsize = STEP_500;
					if (mode == CW) { filt = 4; nr = 0; }
					else filt = 0;
				}
				if (menu == BAND) {
					change = true;
				}
				if (menu == VFOSEL) {
					freq = vfo[vfosel % 2];
					mode = vfomode[vfosel % 2];
					if (mode != CW) stepsize = STEP_1k; else stepsize = STEP_500;
					if (mode == CW) { filt = 4; nr = 0; }
					else filt = 0;
					change = true;
				}
#ifdef RIT_ENABLE
				if (menu == RIT) {
					stepsize = (rit) ? STEP_10 : STEP_500;
					change = true;
				}
#endif
				if (menu == ATT) { // post-handling ATT parameter
					if (dsp_cap == SDR) {
						noInterrupts();
#ifdef SWAP_RX_IQ
						adc_start(1, !(att & 0x01)/*true*/, F_ADC_CONV); admux[0] = ADMUX;
						adc_start(0, !(att & 0x01)/*true*/, F_ADC_CONV); admux[1] = ADMUX;
#else
						adc_start(0, !(att & 0x01)/*true*/, F_ADC_CONV); admux[0] = ADMUX;
						adc_start(1, !(att & 0x01)/*true*/, F_ADC_CONV); admux[1] = ADMUX;
#endif //SWAP_RX_IQ
						interrupts();
					}
					fastdigitalWrite(RX, !(att & 0x02)); // att bit 1 ON: attenuate -20dB by disabling RX line, switching Q5 (antenna input switch) into 100k resistence
					fastpinMode(AUDIO1, (att & 0x04) ? OUTPUT : INPUT); // att bit 2 ON: attenuate -40dB by terminating ADC inputs with 10R
					fastpinMode(AUDIO2, (att & 0x04) ? OUTPUT : INPUT);
				}
				if (menu == SIFXTAL) {
					change = true;
				}
#ifdef TX_ENABLE
				if ((menu == PWM_MIN) || (menu == PWM_MAX)) {
					build_lut();
				}
#endif
				if (menu == CWTONE) {
					if (dsp_cap) { cw_offset = (cw_tone == 0) ? tones[0] : tones[1]; paramAction(SAVE, CWOFF); }
				}
				if (menu == IQ_ADJ) {
					change = true;
				}
#ifdef CAL_IQ
				if (menu == CALIB) {
					if (dsp_cap != SDR) calibrate_iq(); menu = 0;
				}
#endif
#ifdef KEYER
				if (menu == KEY_WPM) {
					loadWPM(keyer_speed);
				}
				if (menu == KEY_MODE) {
					if (keyer_mode == 0) { keyerControl = IAMBICA; }
					if (keyer_mode == 1) { keyerControl = IAMBICB; }
					if (keyer_mode == 2) { keyerControl = SINGLE; }
				}
#endif //KEYER
#ifdef TX_DELAY
				if (menu == TXDELAY) {
					semi_qsk = (txdelay > 0);
				}
#endif //TX_DELAY
			}
#ifdef DEBUG
			if (menu == SR) {          // measure sample-rate
				numSamples = 0;
				delay(F_MCU * 500UL / 16000000);   // delay 0.5s (in reality because F_CPU=20M instead of 16M, delay() is running 1.25x faster therefore we need to multiply with 1.25)
				sr = numSamples * 2;   // samples per second
				paramAction(UPDATE_MENU, menu); // refresh
			}
			if (menu == CPULOAD) {     // measure CPU-load
				uint32_t i = 0;
				uint32_t prev_time = millis();
				for (i = 0; i != 300000; i++) wdt_reset(); // fixed CPU-load 132052*1.25us delay under 0% load condition; is 132052*1.25 * 20M = 3301300 CPU cycles fixed load
				cpu_load = 100 - 132 * 100 / (millis() - prev_time);
				paramAction(UPDATE_MENU, menu); // refresh
			}
			if ((menu == PARAM_A) || (menu == PARAM_B) || (menu == PARAM_C)) {
				delay(300);
				paramAction(UPDATE_MENU, menu); // refresh
			}
#endif
		}
	}

	if (menumode == 0) {
		if (encoder_val) {  // process encoder tuning steps
			process_encoder_tuning_step(encoder_val);
			encoder_val = 0;
		}
	}

	if ((change) && (!tx) && (!vox_tx))  // Only change if TX is OFF, prevent simultaneous I2C bus access
  {
		change = false;
		if (prev_bandval != bandval) {  // If band changed
			prev_bandval = bandval;

#ifdef KEEP_BAND_DATA  // G8RDI mod
			if (freq_last[bandval - 1] != 0)    // G8RDI mod
			{
				freq = freq_last[bandval - 1];  // Change to last freq used on this band
				if (freq > 60000000)  // Keep in range to avoid lots of twiddling
				{
					freq = band[bandval];
					freq_last[bandval - 1] = freq;
				}
			}
			else
				freq = band[bandval];   // Load default
			if (mode_last[bandval - 1] > AM)
			{
				mode_last[bandval - 1] = LSB;  // Should never happen, error! G8RDI mod
				error_code = 3;
			}

      mode = mode_last[bandval - 1];   // Change to last used mode on this band

			if (mode > CW)
				mode = LSB;  // skip all other modes (only LSB (0), USB, CW(2))
#ifdef MODE_CHANGE_RESETS
			if (mode != CW) stepsize = STEP_1k; else stepsize = STEP_500; // sets suitable stepsize
			if (mode == CW) { filt = 4; nr = 0; }
			else filt = 0;  // resets filter (to most BW) and NR on mode change
#else
			if (mode == CW) { nr = 0; }
			prev_stepsize[prev_mode == CW] = stepsize; stepsize = prev_stepsize[mode == CW]; // backup stepsize setting for previous mode, restore previous stepsize setting for current selected mode; filter settings captured for either CQ or other modes.
			prev_filt[prev_mode == CW] = filt; filt = prev_filt[mode == CW];  // backup filter setting for previous mode, restore previous filter setting for current selected mode; filter settings captured for either CQ or other modes.
#endif
#ifdef CW_DECODER
			if (prev_mode == CW && cwdec)
				show_banner();
#endif

#else
			freq = band[bandval];   // Change to new band freq start
#endif
			vfomode[vfosel % 2] = mode;
			paramAction(SAVE, (vfosel % 2) ? MODEB : MODEA);  // save vfoa/b changes
			paramAction(SAVE, MODE);
			paramAction(SAVE, FILTER);
			si5351.iqmsa = 0;  // enforce PLL reset
		}

		vfo[vfosel % 2] = freq;
		save_event_time = millis() + 2000;  // G8RDI mod - increased to 2 seconds // schedule time to save freq (no save while tuning, hence no EEPROM wear out - G8RDI "Datasheet: Write/erase cycles: 10,000 flash/100,000 EEPROM")

		if (menumode == 0) {
			display_vfo(freq);
			stepsize_showcursor();
		}

		uint8_t f = freq / 1000000UL;
		set_lpf(f);
		#ifdef TRUSDX
		#ifdef LOBAND
		bandval = (f > 32) ? 10 : (f > 26) ? 9 : (f > 22) ? 8 : (f > 20) ? 7 : (f > 16) ? 6 : (f > 12) ? 5 : (f > 8) ? 4 : (f > 6) ? 3 : (f > 4) ? 2 : (f > 2) ? 1 : 0;  prev_bandval = bandval; // align bandval with freq
		#endif 
		#ifdef CLASSICBAND
		bandval = (f > 462) ? 10 : (f > 420) ? 9 : (f > 222) ? 8 : (f > 144) ? 7 : (f > 50) ? 6 : (f > 23) ? 5 : (f > 17) ? 4 : (f > 8) ? 3 : (f > 4) ? 2 : (f > 2) ? 1 : 0;  prev_bandval = bandval; // align bandval with freq
		#endif
		#ifdef HIBAND
		bandval = (f > 462) ? 10 : (f > 420) ? 9 : (f > 222) ? 8 : (f > 144) ? 7 : (f > 50) ? 6 : (f > 25) ? 5 : (f > 23) ? 4 : (f > 19) ? 3 : (f > 17) ? 2 : (f > 2) ? 1 : 0;  prev_bandval = bandval; // align bandval with freq
		#endif 
		#else
		bandval = (f > 32) ? 10 : (f > 26) ? 9 : (f > 22) ? 8 : (f > 20) ? 7 : (f > 16) ? 6 : (f > 12) ? 5 : (f > 8) ? 4 : (f > 6) ? 3 : (f > 4) ? 2 : (f > 2) ? 1 : 0;  prev_bandval = bandval; // align bandval with freq
		#endif // TRUSDX
		if (mode == CW) {
			si5351.freq(freq + cw_offset, rx_ph_q, 0/*90, 0*/);  // RX in CW-R (=LSB), correct for CW-tone offset
		}
		else
			if (mode == LSB)
				si5351.freq(freq, rx_ph_q, 0/*90, 0*/);  // RX in LSB
			else
				si5351.freq(freq, 0, rx_ph_q/*0, 90*/);  // RX in USB, ...
#ifdef RIT_ENABLE
		if (rit) { si5351.freq_calc_fast(rit); si5351.SendPLLRegisterBulk(); }
#endif //RIT_ENABLE
	}

	if (save_event_time && (millis() > save_event_time)) {  // save freq when time has reached schedule - reduce EEPROM writes as 10k limit to burnout (G8RDI)!
		paramAction(SAVE, (vfosel % 2) ? FREQB : FREQA);  // save vfoa/b changes

#ifdef KEEP_BAND_DATA  // G8RDI mod
		freq_last[bandval - 1] = vfo[vfosel % 2];       // = freq;
		mode_last[bandval - 1] = vfomode[vfosel % 2];   // = mode;

    if ((bandval - 1) <= 8)
      paramAction(SAVE, BAND_DATA0 + (bandval - 1));  // Save updated data only
    else
      error_code = 1;      // Flag error

#endif
		save_event_time = 0;
	}

#ifdef CW_MESSAGE
	// Auto-send CW message
	if ((mode == CW) && (cw_msg_event) && (millis() > cw_msg_event)) {  // if it is time, send CW message
		if ((cw_tx(cw_msg[cw_msg_id]) == 0) && ((cw_msg[cw_msg_id][0] == 'C') && (cw_msg[cw_msg_id][1] == 'Q')) && cw_msg_interval)     // If msg starts CQ, schedule repeat send at set interval
			cw_msg_event = millis() + (1000 * cw_msg_interval);
		else
			cw_msg_event = 0;  // Done/aborted by key press (if CW aborts can also be caused by noise getting onto keys ADC line)
	}
#endif //CW_MESSAGE

	wdt_reset();
	}