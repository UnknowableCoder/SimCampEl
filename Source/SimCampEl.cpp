/**********************************************
 *                                            *
 *  SimCampEl - Simulador de Campos Elécticos *
 *                                            *
 *     Por: Gonçalo Vaz (87321)               *
 *          Nuno Fernandes (87342)            *
 *                                            *
 **********************************************/

#include "g24_lib.cpp"

#include "SI2.cpp"

using namespace g24_lib;
//Para nos poupar às alterações decorrentes
//da ausência do namespace em iterações anteriores
//da nossa bibliotecazita...

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <locale>
#include <clocale>
#include <codecvt>
#include <sstream>
#include <iomanip>
#include <omp.h>

#include <ctime>
//Para aleatórios e respectivas seeds.

#define FP_USE_THREAD_SAFE_EVAL 1

#define FP_SUPPORT_CPLUSPLUS11_MATH_FUNCS 1

#define FP_SUPPORT_FLOAT_TYPE 1

#include "fparser.hh"
//Uma biblioteca para expressões matemáticas.

#define FLType float
//Uma maneira fácil de trocar entre usar doubles
//ou floats para os cálculos...

#define QE__ 1.6021766208e-19
//A carga elementar do electrão.
#define C__ 299792458.0
//A velocidade da luz no vazio.
#define COULCONS__ 8.9875517873681764e9
//A constante de Coulomb: 1/(4 pi epsilon_0)
#define BISAVCONS__ 1e-7
//A constante de Biot-Savart: mu_0/(4 pi)

#define ALTERNATE_COLOR_GEN 0
//Altera o algoritmo usado para atribuir as cores às cargas
//para as suas representações

#include "irrlicht.h"
//Para a simulação gráfica...
#include "driverchoice.h"
//Para permitir a escolha das drivers gráficas a usar...

#include "CGUIFileSelector.h"
#include "CGUIFileSelector.cpp"
//Código adaptado do projecto open-source IRRRPGBuilder,
//por Christian Clavet, que permite um selector de ficheiros
//mais user-friendly do que o default de Irrlicht....

#include "IGUIColorPicker.h"
#include "IGUIColorPicker.cpp"
//Código disponibilizado por Ovan nos fóruns da biblioteca Irrlicht
//que permite um selector de cores propriamente dito.


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
//Embora o uso sistemático de "using namespace"
//seja vagamente reprovável, no sentido em que
//acaba por anular o propósito dos namespaces,
//dada a complexidade da estrutura de namespaces
//da biblioteca gráfica utilizada, será, neste caso,
//mais do que justificado pela abreviação providenciada...

enum Keys { Forward = 0, Backward, Left, Right, Up, Down, Velocity,
            Acceleration, Angle, Stop, Reset, BackToOrigin, PlayPause,
            RelativeSwitch, OneStep, TOTALKEYS };
//Para o input, no caso da simulação interactiva...

std::wstring keytype_to_string(int k)
{
  switch (k)
    {
    default:
      return L"NO KEY";
    case Forward:
      return L"Move Camera Forward";
    case Backward:
      return L"Move Camera Backward";
    case Left:
      return L"Move Camera Left";
    case Right:
      return L"Move Camera Right";
    case Up:
      return L"Move Camera Up";
    case Down:
      return L"Move Camera Down";
    case Velocity:
      return L"Modify Velocity";
    case Acceleration:
      return L"Modify Acceleration";
    case Angle:
      return L"Modify Angles";
    case Stop:
      return L"Stop Camera Motion";
    case Reset:
      return L"Reset Camera Angle";
    case BackToOrigin:
      return L"Reset Camera Position";
    case PlayPause:
      return L"Play and Pause Simulation";
    case RelativeSwitch:
      return L"Switch Relative Movement";
    case OneStep:
      return L"Simulate One Step";
    }
}

std::wstring key_to_name(int code)
//Devolve o nome apropriado para cada tecla.
{
  switch (code)
    {
    default:
      return L"Unknown";
    case KEY_LBUTTON: // Left mouse button
      return L"Left Mouse Button";
    case KEY_RBUTTON: // Right mouse button
      return L"Right Mouse Button";
    case KEY_CANCEL: // Control-break processing
      return L"Cancel";
    case KEY_MBUTTON: // Middle mouse button (three-button mouse)
      return L"Middle Mouse Button";
    case KEY_XBUTTON1: // Windows 2/XP: X1 mouse button
      return L"X1 Mouse Button";
    case KEY_XBUTTON2: // Windows 2/XP: X2 mouse button
      return L"X2 Mouse Button";
    case KEY_BACK: // BACKSPACE key
      return L"Backspace";
    case KEY_TAB: // TAB key
      return L"Tab";
    case KEY_CLEAR: // CLEAR key
      return L"Clear";
    case KEY_RETURN: // ENTER key
      return L"Enter";
    case KEY_SHIFT: // SHIFT key
      return L"Shift";
    case KEY_CONTROL: // CTRL key
      return L"Control";
    case KEY_MENU: // ALT key
      return L"Alt";
    case KEY_PAUSE: // PAUSE key
      return L"Pause / Break";
    case KEY_CAPITAL: // CAPS LOCK key
      return L"Caps Lock";
    case KEY_KANA: // IME Kana mode or IME Hangul mode
      return L"IME Kana & Hangul Modes";
    case KEY_JUNJA: // IME Junja mode
      return L"IME Junja Mode";
    case KEY_FINAL: // IME final mode
      return L"IME Final Mode";
    case KEY_HANJA: // IME Hanja mode or IME Kanji mode
      return L"IME Hanja & Kanji Mode";
    case KEY_ESCAPE: // ESC key
      return L"Escape";
    case KEY_CONVERT: // IME convert
      return L"IME Convert";
    case KEY_NONCONVERT: // IME nonconvert
      return L"IME Non-Convert";
    case KEY_ACCEPT: // IME accept
      return L"IME Accept";
    case KEY_MODECHANGE: // IME mode change request
      return L"IME Mode Change Request";
    case KEY_SPACE: // SPACEBAR
      return L"Spacebar";
    case KEY_PRIOR: // PAGE UP key
      return L"Page Up";
    case KEY_NEXT: // PAGE DOWN key
      return L"Page Down";
    case KEY_END: // END key
      return L"End";
    case KEY_HOME: // HOME key
      return L"Home";
    case KEY_LEFT: // LEFT ARROW key
      return L"Left Arrow";
    case KEY_UP: // UP ARROW key
      return L"Up Arrow";
    case KEY_RIGHT: // RIGHT ARROW key
      return L"Right Arrow";
    case KEY_DOWN: // DOWN ARROW key
      return L"Down Arrow";
    case KEY_SELECT: // SELECT key
      return L"Select";
    case KEY_PRINT: // PRINT key
      return L"Print";
    case KEY_EXECUT: // EXECUTE key
      return L"Execute";
    case KEY_SNAPSHOT: // PRINT SCREEN key
      return L"Print Screen";
    case KEY_INSERT: // INS key
      return L"Insert";
    case KEY_DELETE: // DEL key
      return L"Delete";
    case KEY_HELP: // HELP key
      return L"Help";
    case KEY_KEY_0: // 0 key
      return L"0";
    case KEY_KEY_1: // 1 key
      return L"1";
    case KEY_KEY_2: // 2 key
      return L"2";
    case KEY_KEY_3: // 3 key
      return L"3";
    case KEY_KEY_4: // 4 key
      return L"4";
    case KEY_KEY_5: // 5 key
      return L"5";
    case KEY_KEY_6: // 6 key
      return L"6";
    case KEY_KEY_7: // 7 key
      return L"7";
    case KEY_KEY_8: // 8 key
      return L"8";
    case KEY_KEY_9: // 9 key
      return L"9";
    case KEY_KEY_A: // A key
      return L"A";
    case KEY_KEY_B: // B key
      return L"B";
    case KEY_KEY_C: // C key
      return L"C";
    case KEY_KEY_D: // D key
      return L"D";
    case KEY_KEY_E: // E key
      return L"E";
    case KEY_KEY_F: // F key
      return L"F";
    case KEY_KEY_G: // G key
      return L"G";
    case KEY_KEY_H: // H key
      return L"H";
    case KEY_KEY_I: // I key
      return L"I";
    case KEY_KEY_J: // J key
      return L"J";
    case KEY_KEY_K: // K key
      return L"K";
    case KEY_KEY_L: // L key
      return L"L";
    case KEY_KEY_M: // M key
      return L"M";
    case KEY_KEY_N: // N key
      return L"N";
    case KEY_KEY_O: // O key
      return L"O";
    case KEY_KEY_P: // P key
      return L"P";
    case KEY_KEY_Q: // Q key
      return L"Q";
    case KEY_KEY_R: // R key
      return L"R";
    case KEY_KEY_S: // S key
      return L"S";
    case KEY_KEY_T: // T key
      return L"T";
    case KEY_KEY_U: // U key
      return L"U";
    case KEY_KEY_V: // V key
      return L"V";
    case KEY_KEY_W: // W key
      return L"W";
    case KEY_KEY_X: // X key
      return L"X";
    case KEY_KEY_Y: // Y key
      return L"Y";
    case KEY_KEY_Z: // Z key
      return L"Z";
    case KEY_LWIN: // Left Windows key (Microsoft� Natural� keyboard)
      return L"Left Windows Key";
    case KEY_RWIN: // Right Windows key (Natural keyboard)
      return L"Right Windows Key";
    case KEY_APPS: // Applications key (Natural keyboard)
      return L"Apps Key";
    case KEY_SLEEP: // Computer Sleep key
      return L"Sleep";
    case KEY_NUMPAD0: // Numeric keypad 0 key
      return L"Numpad 0";
    case KEY_NUMPAD1: // Numeric keypad 1 key
      return L"Numpad 1";
    case KEY_NUMPAD2: // Numeric keypad 2 key
      return L"Numpad 2";
    case KEY_NUMPAD3: // Numeric keypad 3 key
      return L"Numpad 3";
    case KEY_NUMPAD4: // Numeric keypad 4 key
      return L"Numpad 4";
    case KEY_NUMPAD5: // Numeric keypad 5 key
      return L"Numpad 5";
    case KEY_NUMPAD6: // Numeric keypad 6 key
      return L"Numpad 6";
    case KEY_NUMPAD7: // Numeric keypad 7 key
      return L"Numpad 7";
    case KEY_NUMPAD8: // Numeric keypad 8 key
      return L"Numpad 8";
    case KEY_NUMPAD9: // Numeric keypad 9 key
      return L"Numpad 9";
    case KEY_MULTIPLY: // Multiply key
      return L"Numpad *";
    case KEY_ADD: // Add key
      return L"Numpad +";
    case KEY_SEPARATOR: // Separator key
      return L"Separator";
    case KEY_SUBTRACT: // Subtract key
      return L"Numpad -";
    case KEY_DECIMAL: // Decimal key
      return L"Numpad .";
    case KEY_DIVIDE: // Divide key
      return L"Numpad /";
    case KEY_F1: // F1 key
      return L"F1";
    case KEY_F2: // F2 key
      return L"F2";
    case KEY_F3: // F3 key
      return L"F3";
    case KEY_F4: // F4 key
      return L"F4";
    case KEY_F5: // F5 key
      return L"F5";
    case KEY_F6: // F6 key
      return L"F6";
    case KEY_F7: // F7 key
      return L"F7";
    case KEY_F8: // F8 key
      return L"F8";
    case KEY_F9: // F9 key
      return L"F9";
    case KEY_F10: // F10 key
      return L"F10";
    case KEY_F11: // F11 key
      return L"F11";
    case KEY_F12: // F12 key
      return L"F12";
    case KEY_F13: // F13 key
      return L"F13";
    case KEY_F14: // F14 key
      return L"F14";
    case KEY_F15: // F15 key
      return L"F15";
    case KEY_F16: // F16 key
      return L"F16";
    case KEY_F17: // F17 key
      return L"F17";
    case KEY_F18: // F18 key
      return L"F18";
    case KEY_F19: // F19 key
      return L"F19";
    case KEY_F20: // F20 key
      return L"F20";
    case KEY_F21: // F21 key
      return L"F21";
    case KEY_F22: // F22 key
      return L"F22";
    case KEY_F23: // F23 key
      return L"F23";
    case KEY_F24: // F24 key
      return L"F24";
    case KEY_NUMLOCK: // NUM LOCK key
      return L"Number Lock";
    case KEY_SCROLL: // SCROLL LOCK key
      return L"Scroll Lock";
    case KEY_LSHIFT: // Left SHIFT key
      return L"Left Shift";
    case KEY_RSHIFT: // Right SHIFT key
      return L"Right Shift";
    case KEY_LCONTROL: // Left CONTROL key
      return L"Left Control";
    case KEY_RCONTROL: // Right CONTROL key
      return L"Right Control";
    case KEY_LMENU: // Left MENU key
      return L"Left Alt";
    case KEY_RMENU: // Right MENU key
      return L"Right Alt";
    case KEY_OEM_1: // for US    ";:"
      //Teclado PT: ´`
      return L"´`";
    case KEY_PLUS: // Plus Key   "+"
      return L"+";
    case KEY_COMMA: // Comma Key  ","
      return L",";
    case KEY_MINUS: // Minus Key  "-"
      return L"-";
    case KEY_PERIOD: // Period Key "."
      return L".";
    case KEY_OEM_2: // for US    "/?"
      return L"~^";
    case KEY_OEM_3: // for US    "`~"
      return L"Ç";
    case KEY_OEM_4: // for US    "[{"
      return L"?'";
    case KEY_OEM_5: // for US    "\|"
      return L"\\|";
    case KEY_OEM_6: // for US    "]}"
      return L"«»";
    case KEY_OEM_7: // for US    "'""
      return L"ºª";
    case KEY_OEM_8: // None
      return L"Unknown";
    case KEY_OEM_AX: // for Japan "AX"
      return L"AX Key";
    case KEY_OEM_102: // "<>" or "\|"
      return L"<>";
    case KEY_ATTN: // Attn key
      return L"Attention Key";
    case KEY_CRSEL: // CrSel key
      return L"Cursor Select Key";
    case KEY_EXSEL: // ExSel key
      return L"Extend Selection Key";
    case KEY_EREOF: // Erase EOF key
      return L"Erase to End of Field Key";
    case KEY_PLAY: // Play key
      return L"Play Key";
    case KEY_ZOOM: // Zoom key
      return L"Zoom Key";
    case KEY_PA1: // PA1 key
      return L"PA1 Key";
    case KEY_OEM_CLEAR: // Clear key
      return L"OEM Clear Key";
    }
}

enum GuiID {  GUITab = -1000, GUIText,

              GUIPlayPause, GUIRecord,
              GUIRecordNum, GUIRecordInterval,
              GUIScrollTime, GUIScrollSpace,
              GUIEditTime, GUIEditSpace,

              GUILoadDialog, GUISaveDialog,
              GUIOptSaveDialog, GUIOptLoadDialog,
              GUIDataSaveDialog, GUISimpleLoadDialog,

              GUIArrow1, GUIArrow2, GUIArrow3,
              GUIArrow4, GUIArrow5, GUIArrow6,

              GUISphere,
              GUICyl1, GUICyl2, GUICyl3,

              GUIQEditor,
              GUIQEChoose, GUIQELabel, GUIQEColorButton,
              GUIQEColor, GUIQEPosX, GUIQEPosY,
              GUIQEPosZ, GUIQEVelX, GUIQEVelY,
              GUIQEVelZ, GUIQEMomX, GUIQEMomY,
              GUIQEMomZ, GUIQERadius, GUIQEMass,
              GUIQECharge, GUIQEDelete, GUIQEClone,
              GUIQEMove, GUIQEDeformable, GUIQEPolarizability,
              GUIQEPartCharge,

              GUISimparEditor,
              GUISimparScrollTime, GUISimparLowLimitsX,
              GUISimparLowLimitsY, GUISimparLowLimitsZ,
              GUISimparUpLimitsX, GUISimparUpLimitsY,
              GUISimparUpLimitsZ, GUISimparEExtX, GUISimparEExtY,
              GUISimparEExtZ, GUISimparBExtX, GUISimparBExtY,
              GUISimparBExtZ, GUISimparLightspeed,
              GUISimparHistSize, GUISimparAutosizeHist,
              GUISimparBGCol, GUISimparBGColButton,
              GUISimparBndCol, GUISimparBndColButton,

              GUICamEditor,
              GUICamPosX, GUICamPosY, GUICamPosZ,
              GUICamVelX, GUICamVelY, GUICamVelZ,
              GUICamAccX, GUICamAccY, GUICamAccZ,
              GUICamAPosX, GUICamAPosY, GUICamAPosZ,
              GUICamAVelX, GUICamAVelY, GUICamAVelZ,
              GUICamAAccX, GUICamAAccY, GUICamAAccZ,
              GUICamScrollSpace,

              GUIKeybinding,
              GUIKey0,
              GUIDummy = GUIKey0+TOTALKEYS+1,

              GUIRecordWindow,
              GUIRWFileSelect, GUIRWFilePath,

              GUIOptions,
              GUIOptITCScroll, GUIOptITCEdit,
              GUIOptInvx, GUIOptInvy,
              GUIOptROCScroll, GUIOptROCEdit,
              GUIOptRelMov, GUIOptThreadScroll,
              GUIOptThreadEdit,

              GUIHelp,

              GUICredits,

              MenuFile, MenuEdit,
              MenuView, MenuOptions, MenuAbout,
              MenuFNew, DummySep1,
              MenuFOpen, MenuFOpenSimple, DummySep2,
              MenuFSave, MenuFSaveAs,
              DummySep3, MenuFExit,
              MenuESimPar, DummySep4, MenuECharges,
              DummySep5, MenuEClearHist,
              MenuVCam, DummySep6, MenuVRec,
              MenuVRecing, DummySepE, MenuVPlayPause,
              MenuOComp, MenuOKeys, DummySep7,
              MenuOSave, MenuOSaveAs, MenuOLoad,
              DummySep8, MenuOSetDefault, DummySep9,
              MenuOReset, MenuAHelp, DummySep10,
              MenuACreds,

              GUIMouseMenu,
              MMenuSelect, MMenuStop, MMenuMove,
              DummySepA, MMenuClone, DummySepB,
              MMenuEdit, DummySepC, MMenuDelete,

              ToolbarMove, ToolbarStop, ToolbarEdit,
              ToolbarAdd, ToolbarClone, ToolbarDelete, ToolbarLast  };
//A identificação dos diversos elementos do GUI.
//(Essencialmente, para facilitar a detecção de quais
//foram activados pelo utilizador...)

float & getv3df (irr::core::vector3df &v, const size_t i)
//Permite obter um elemento de um vector
//a partir de um número.
{
  switch(i)
    {
    default:
    case 0:
      return v.X;
    case 1:
      return v.Y;
    case 2:
      return v.Z;
    }
}

float getv3df (const irr::core::vector3df &v, const size_t i)
//Permite obter um elemento de um vector
//a partir de um número.
{
  switch(i)
    {
    default:
    case 0:
      return v.X;
    case 1:
      return v.Y;
    case 2:
      return v.Z;
    }
}

fspoint<FLType> irr_to_ours(const irr::core::vector3df &v)
{
  fspoint<FLType> ret;
  ret[0] = v.X;
  ret[1] = v.Y;
  ret[2] = v.Z;
  return ret;
}

struct charge;
//Forward declaration...

/***************************************

                CHARGE_REC

****************************************/

struct charge_rec
{
  fspoint<FLType> pos, vel, acc;
  FLType time, m, q;

  charge_rec()
  {
    return;
  }

  charge_rec (const charge &);

  void initialize()
  {
    pos.set_all(0);
    vel.set_all(0);
    acc.set_all(0);
    m = 0;
    q = 0;
#if FLType == float
    time = std::nanf("");
#else
    time = std::nan("");
#endif
  }

  bool initialized()
  {
    return std::isnan(time);
  }

  charge_rec operator+ (const charge_rec &c) const
  {
    charge_rec temp(*this);
    temp.pos += c.pos;
    temp.vel += c.vel;
    temp.acc += c.acc;
    temp.time += c.time;
    temp.m += c.m; //TV?!
    temp.q += c.q;
    return temp;
  }
  charge_rec operator- (const charge_rec &c) const
  {
    charge_rec temp(*this);
    temp.pos -= c.pos;
    temp.vel -= c.vel;
    temp.acc -= c.acc;
    temp.time -= c.time;
    temp.m -= c.m; //TV?!
    temp.q -= c.q;
    return temp;
  }

  charge_rec operator- () const
  {
    charge_rec temp(*this);
    temp.pos = -temp.pos;
    temp.vel = -temp.vel;
    temp.acc = -temp.acc;
    temp.time = -temp.time;
    temp.m = -temp.m; //TV?!
    temp.q = -temp.q;
    return temp;
  }

  charge_rec& operator+= (const charge_rec& c)
  {
    pos += c.pos;
    vel += c.vel;
    acc += c.acc;
    time += c.time;
    m += c.m; //TV?!
    q += c.q;
    return (*this);
  }

  charge_rec& operator-= (const charge_rec& c)
  {
    pos -= c.pos;
    vel -= c.vel;
    acc -= c.acc;
    time -= c.time;
    m -= c.m; //TV?!
    q -= c.q;
    return (*this);
  }

  charge_rec& operator*= (const FLType a)
  {
    pos = pos*a;
    vel = vel*a;
    acc = acc*a;
    time = time*a;
    m = m*a;
    q = q*a;
    return (*this);
  }

  charge_rec& operator/= (const FLType a)
  {
    pos = pos/a;
    vel = vel/a;
    acc = acc/a;
    time = time/a;
    m = m/a;
    q = q/a;
    return (*this);
  }

  charge_rec operator* (const FLType a) const
  {
    charge_rec temp(*this);
    temp.pos = temp.pos*a;
    temp.vel = temp.vel*a;
    temp.acc = temp.acc*a;
    temp.time = temp.time*a;
    temp.m = temp.m*a;
    temp.q = temp.q*a;
    return temp;
  }

  charge_rec operator/ (const FLType a) const
  {
    charge_rec temp(*this);
    temp.pos = temp.pos/a;
    temp.vel = temp.vel/a;
    temp.acc = temp.acc/a;
    temp.time = temp.time/a;
    temp.m = temp.m/a;
    temp.q = temp.q/a;
    return temp;
  }

};

std::istream& operator>> (std::istream& s, charge_rec& cr)
{
  cr.pos.raw_read(s);
  cr.vel.raw_read(s);
  cr.acc.raw_read(s);
  std::string temp;
  s >> temp;
  cr.time = std::strtod(temp.c_str(), nullptr);
  s >> cr.m >> cr.q;
  return s;
}

std::ostream& operator<< (std::ostream& s, const charge_rec& cr)
{
  cr.pos.raw_print(s);
  cr.vel.raw_print(s);
  cr.acc.raw_print(s);
  s << cr.time << " " << cr.m << " " << cr.q;
  return s;
}

/***************************************

                   PROPS

****************************************/

struct props
//Para aglomerar algumas propriedades do espaço da simulação,
//como planos de potencial ou outro tipo de adições,
//bem como várias coisas úteis para a simulação tridimensional.
{

  size_t printerval, totalprint;
  //Para as questões da impressão...
  //printerval é o número de iterações/frames
  //a realizar antes de cada impressão,
  //totalprint é o número total de impressões.

  FLType limits[6];
  //Os limites da região de simulação em causa
  //(para que as partículas não fujam para o infinito
  // no caso mais geral de n partículas a interagir...)

  std::ofstream out;
  //Para o output dos dados...

  FLType timetotal;
  //O tempo total que já dura a simulação...

  FLType lightspeed;
  //A velocidade da luz.

  std::vector<charge> simarr;

  history<charge_rec> hist;

  IrrlichtDevice *device;
  irr::video::IVideoDriver *driver;
  irr::scene::ISceneManager *smgr;
  irr::gui::IGUIEnvironment *guienv;
  //Pointers utilitários para lidar com as propriedades da biblioteca gráfica...

  irr::core::dimension2du windowsize;
  //O tamanho da janela de visualização.

  irr::scene::ICameraSceneNode *camera;
  //A câmara.

  irr::video::SColor background;
  //A cor do pano de fundo da simulação.

  irr::core::vector3df c_lpos, c_lvel, c_lacc, c_avel, c_aacc;
  //Posição e velocidades e acelerações lineares e angulares da câmara.

  irr::core::vector3df lookatv, upv, sidev;
  //Três vectores para representarem os eixos do referencial próprio da partícula.

  irr::core::quaternion rot;
  //Representa a quantidade rodada em torno dos eixos.

  void update_cam_vectors()
  {
    lookatv.set(0,0,1);
    upv.set(0,1,0);
    sidev.set(1,0,0);
    lookatv = rot*lookatv;
    upv = rot*upv;
    sidev = rot*sidev;
  }

  float rate_of_change, past_roc;
  //Controla a facilidade com que se alteram os parâmetros da câmara.

  bool invertx, inverty;
  //Permite inverter a forma de alterar os ângulos
  //(no caso de ser mais natural ao utilizador)

  bool relative;
  //Permite alternar entre um referencial absoluto ou relativo
  //no que toca aos movimentos da câmara.

  float scale, past_scale;
  //A escala da representação...

  irr::s32 selected;
  //Indica a esfera seleccionada
  //(para as ferramentas)

  bool play;
  //Indica se a evolução temporal está em execução...

  EKEY_CODE keysettings[TOTALKEYS];
  //Guarda as teclas atribuídas a cada acção...

  float deltat;
  //O intervalo de tempo entre cada frame
  //(não necessariamente igual ao inverso da framerate,
  //posto que esta só é actualizada esporadicamente pela biblioteca!)

  irr::scene::IMeshSceneNode *bounds[6];
  //Os planos que delimitarão a zona de visualização...

  irr::video::SColor boundcolor;
  //A cor desses mesmos planos...

  float timescale, past_timescale;
  //A escala de tempo da visualização.
  //(Alterável com ângulo + roda do rato)

  size_t itercount;
  //Para permitir melhorar a precisão da simulação,
  //torna-se possível efectuar mais do que uma iteração
  //por frame (com maior custo computacional, naturalmente)

  size_t numthreads;
  //Para melhorar a eficiência, ou a rapidez,
  //dá-se a possibilidade de aproveitar o multithreading
  //nativo do C++ para acelerar os cálculos da evolução temporal.

  std::vector<fspoint<FLType>> temp_earr;
  //Para aglomerar valores temporários de campo,
  //usados na representação gráfica das cargas.

  SII::SI_interpreter<FLType, std::wstring> interpreter;
  //Um interpretador das unidades SI no input!

  std::string Eext[3], Bext[3];
  //Campos eléctricos e magnéticos externos...

  FunctionParser_f Efunc[3], Bfunc[3];

  irr::gui::IGUIButton *plpausb, *recb;

  irr::gui::IGUIScrollBar *timescroll, *spacescroll;
  irr::gui::IGUIStaticText *indicativetexts[15];
  irr::gui::IGUIEditBox *timedit, *spacedit, *recordnum, *recordint;

  irr::gui::IGUIWindow *helpwindow, *optionwindow, *simpareditor,
    *chargeditor, *cameditor, *keywindow,
    *creditwindow, *recordwindow;
  CGUIFileSelector *opendialog;

  irr::gui::IGUIContextMenu* menu, *mousemenu;
  irr::gui::IGUIToolBar *toolbar;

  irr::gui::IGUIButton *toolbarbuttons[ToolbarLast-ToolbarMove];

  irr::gui::IGUITab *lowermenu;

  irr::gui::IGUIStaticText *lowermenutime;

  //Diversos elementos do GUI, com funções mais ou menos óbvias...

  irr::scene::IMesh* arm[6];
  irr::scene::IMeshSceneNode *arrow[6];

  irr::s32 toolinuse;
  //Para indicar qual das ferramentas da toolbar
  //está seleccionada

  bool blockinteract;
  //Para que os inputs do teclado não influenciem
  //a vista principal quando há janelas abertas.

  irr::s32 mmenusel;

  std::string loaded, loadedsettings;
  //Para permitir save & save as

  std::wstring recsave;
  //Para saber o ficheiro onde se está a guardar as coisas.

  bool changed, changedsettings;
  //Para assinalar se houve alterações
  //no ficheiro de configurações ou não.

  vector3df *vertexpos;
  //Guarda a posição original dos vértices numa esfera não deformada.


  void change_selected(const irr::s32 new_selected);
  //Forward declaration, porque se necessita de conhecer os membros de charge...

  void delete_charge(const size_t index);

  void add_charge(const charge& newq);

  void create_help_window()
  {
    static const s32 HELPWINDOWH = 700, HELPWINDOWW = 700;
    helpwindow = guienv->addWindow(rect<s32>(0, 0, HELPWINDOWW, HELPWINDOWH), false, L"Help", 0, GUIHelp);
    guienv->addStaticText(L"This is the help window for SimCampEl.\n\nThough a more comprehensive "
                          L"guide can be found in the accompanying manual (SimCampEl.pdf), "
                          L"the main actions the user might want to take can be done through "
                          L"the keys hereby indicated:"
                          L"Position-related key:\n\n"
                          L" - To move the camera forward/backward, press W/S or use the scroll.\n"
                          L" - To move the camera to the left/right, press A/D.\n"
                          L" - To move the camera upward/downward, press Q/E.\n"
                          L" - To control the camera's angle, press LEFT ALT and the key that corresponds "
                          L"to the direction in which the camera should turn.\n\n"
                          L"The previous commands can be modified by the LEFT CTRL key, to "
                          L"change the camera's velocity, or the LEFT SHIFT key, to change its acceleration.\n\n"
                          L"Other movement-related keys:\n\n"
                          L" - To return the camera to the origin of the referential (0, 0, 0), press HOME.\n"
                          L" - To return the camera to the original angle (facing forward), press BACKSPACE.\n"
                          L" - To toggle between relative and absolute movement of the camera, press R.\n"
                          L" - To reset the velocity and acceleration to 0, press Z.\n\n\n"
                          L"Simulation-related keys:\n\n"
                          L" - To play or pause the simulation, press SPACE.\n"
                          L" - To advance a single step in the simulation, press RETURN.\n\n\n"
                          L"Do note that, when data is being recorded, no charges can be added or removed, "
                          L"so as not to compromise the output file.",
                          rect<s32>(20, 20, HELPWINDOWW - 20, HELPWINDOWH - 20), false, true, helpwindow, GUIText );
    helpwindow->setRelativePosition( rect<s32>( windowsize.Width/2 - HELPWINDOWW/2,
                                                windowsize.Height/2 - HELPWINDOWH/2,
                                                windowsize.Width/2 + HELPWINDOWW/2,
                                                windowsize.Height/2 + HELPWINDOWH/2 ) );
    helpwindow->setVisible(true);
  }

  void create_credits_window()
  {
#define CREDWINDOWW 400
#define CREDWINDOWH 500
    creditwindow = guienv->addWindow(rect<s32>(0, 0, CREDWINDOWW, CREDWINDOWH), false, L"Credits", 0, GUICredits);
    guienv->addStaticText(L"   SimCampEl:\n"
                          L"Nuno Fernandes & Gonçalo Vaz\n\n\n"
                          L"   SII - SI Units Interpreter:\n"
                          L"Main Coder: Nuno Fernandes & Gonçalo Vaz\n\n\n"
                          L"   Irrlicht:\n"
                          L"Michael Zeilfelder, Patryk Nadrowski, Yoran, Dario Oliveri, "
                          L"Thomas Alten & Daniel Bocksteger\n\n"
                          L"(As in http://irrlicht.sourceforge.net/?page_id=301)\n\n\n"
                          L"   FParser:\n"
                          L"Juha Nieminen & Joel Yliluoma \n"
                          L"(http://warp.povusers.org/FunctionParser/)",
                          rect<s32>(20, 20, CREDWINDOWW - 20, CREDWINDOWH - 20), false, true, creditwindow, GUIText );
    creditwindow->setRelativePosition( rect<s32>( windowsize.Width/2 - CREDWINDOWW/2,
                                                  windowsize.Height/2 - CREDWINDOWH/2,
                                                  windowsize.Width/2 + CREDWINDOWW/2,
                                                  windowsize.Height/2 + CREDWINDOWH/2 ) );
    creditwindow->setVisible(true);
  }

  irr::gui::IGUIComboBox *qedit_cb;
  irr::gui::IGUIEditBox *qedit_label, *qedit_pos[3],
       *qedit_vel[3], *qedit_mom[3],
       *qedit_radius, *qedit_mass,
       *qedit_charge, *qedit_polariz;
  irr::gui::IGUIButton *qedit_colorbutton, *qedit_delete, *qedit_clone, *qedit_move;
  irr::gui::IGUIImage *qedit_colorbuttonimg;
  irr::gui::IGUIColorPicker *qedit_color;
  irr::gui::IGUICheckBox *qedit_deformable;
  irr::gui::IGUIScrollBar *qedit_partcharge;
  void update_qecombobox();
  void create_chargeditor()
  {
#define QEDITWINDOWW 660
    //Não, não lhe vou somar 6.
#define QEDITWINDOWH 430
    chargeditor = guienv->addWindow(rect<s32>(0, 0, QEDITWINDOWW, QEDITWINDOWH), false, L"Charge Editor", 0, GUIQEditor);
    qedit_cb = guienv->addComboBox(rect<s32>(20,20,QEDITWINDOWW - 20, 50), chargeditor, GUIQEChoose);
    qedit_cb->addItem(L"");
    guienv->addStaticText(L"Label:",rect<s32>(20,70,80,90),
                          false, true, chargeditor, GUIText          )->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_CENTER);
    qedit_label = guienv->addEditBox(L"",rect<s32>(80,70,QEDITWINDOWW-150,90),true, chargeditor, GUIQELabel );
    guienv->addStaticText(L"Colour:",rect<s32>(QEDITWINDOWW-130,70,QEDITWINDOWW-70,90),
                          false, true, chargeditor, GUIText          )->setTextAlignment(EGUIA_UPPERLEFT,EGUIA_CENTER);
    qedit_colorbutton = guienv->addButton(rect<s32>(QEDITWINDOWW-70,70,QEDITWINDOWW-20,90), chargeditor, GUIQEColorButton);
    qedit_colorbuttonimg = guienv->addImage(core::rect<s32>(5,5,45,15),qedit_colorbutton,-1, L"",false);
    qedit_colorbuttonimg->setImage(guienv->getVideoDriver()->addTexture({1, 1}, "qeditcb", video::ECF_A8R8G8B8));
    qedit_colorbuttonimg->setScaleImage(true);

#define MAKE_3D_EDITS(Name, FirstY, GUIElementArray, IDLabel, Parent, Width) \
    guienv->addStaticText(Name,rect<s32>(20,FirstY,Width-20, FirstY+20), \
                          false, true, Parent, GUIText          )->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT); \
    guienv->addStaticText(L"x:", rect<s32>(20,FirstY+30,80,FirstY+50),false, true, Parent, GUIText); \
    guienv->addStaticText(L"y:", rect<s32>((Width)/3 + 10,FirstY+30,(Width)/3+70,FirstY+50),false, true, Parent, GUIText); \
    guienv->addStaticText(L"z:", rect<s32>(2 * (Width)/3,FirstY+30,2 * (Width)/3+60,FirstY+50),false, true, Parent, GUIText); \
    GUIElementArray[0] = guienv->addEditBox(L"", rect<s32>(40,FirstY+30,(Width)/3,FirstY+50),true,Parent,IDLabel ## X); \
    GUIElementArray[1] = guienv->addEditBox(L"", rect<s32>((Width)/3+30,FirstY+30, 2 * (Width)/3-10,FirstY+50),true,Parent,IDLabel ## Y); \
    GUIElementArray[2] = guienv->addEditBox(L"", rect<s32>(2 * (Width)/3+20,FirstY+30,Width-20,FirstY+50),true,Parent,IDLabel ## Z);

#define THREEDEDIT_SPACING 50
    //Tem de se dar um espaçamento de 50 píxeis para cada um destes blocos, além da separação, claro.


    MAKE_3D_EDITS(L"Position",105,qedit_pos, GUIQEPos,chargeditor, QEDITWINDOWW);
    MAKE_3D_EDITS(L"Velocity",120+THREEDEDIT_SPACING,qedit_vel, GUIQEVel,chargeditor, QEDITWINDOWW);
    MAKE_3D_EDITS(L"Momentum",135+2*THREEDEDIT_SPACING,qedit_mom, GUIQEVel,chargeditor, QEDITWINDOWW);

    guienv->addStaticText(L"Radius:", rect<s32>(20,155+3*THREEDEDIT_SPACING,80,175+3*THREEDEDIT_SPACING),
                          false, true, chargeditor, GUIText);

    guienv->addStaticText(L"Mass:",
                          rect<s32>((QEDITWINDOWW)/3 + 10,155+3*THREEDEDIT_SPACING,(QEDITWINDOWW)/3+70,175+3*THREEDEDIT_SPACING),
                          false, true, chargeditor, GUIText);

    guienv->addStaticText(L"Charge:",
                          rect<s32>(2 * (QEDITWINDOWW)/3,155+3*THREEDEDIT_SPACING,2 * (QEDITWINDOWW)/3+60,175+3*THREEDEDIT_SPACING),
                          false, true, chargeditor, GUIText);

    qedit_radius = guienv->addEditBox(L"",
                                      rect<s32>(80,155+3*THREEDEDIT_SPACING,(QEDITWINDOWW)/3,175+3*THREEDEDIT_SPACING),
                                      true,chargeditor,GUIQERadius);

    qedit_mass = guienv->addEditBox(L"",
                                    rect<s32>((QEDITWINDOWW)/3+70,155+3*THREEDEDIT_SPACING, 2 * (QEDITWINDOWW)/3-10,175+3*THREEDEDIT_SPACING),
                                    true,chargeditor,GUIQEMass);

    qedit_charge = guienv->addEditBox(L"",
                                      rect<s32>(2 * (QEDITWINDOWW)/3+60,155+3*THREEDEDIT_SPACING,QEDITWINDOWW-20,175+3*THREEDEDIT_SPACING),
                                      true,chargeditor,GUIQECharge);

    guienv->addStaticText(L"Deformable?", rect<s32>(20,195+3*THREEDEDIT_SPACING,120,215+3*THREEDEDIT_SPACING),
                          false, false, chargeditor,GUIText);
    qedit_deformable = guienv->addCheckBox(false,
                                           rect<s32>(130,195+3*THREEDEDIT_SPACING,170,215+3*THREEDEDIT_SPACING),
                                           chargeditor, GUIQEDeformable);

    guienv->addStaticText(L"Deformed Charge:", rect<s32>(180,195+3*THREEDEDIT_SPACING,310,215+3*THREEDEDIT_SPACING),
                          false, false, chargeditor,GUIText);
    (guienv->addStaticText(L"-", rect<s32>(305,195+3*THREEDEDIT_SPACING,315,215+3*THREEDEDIT_SPACING),
                           false, false, chargeditor,GUIText))->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT);
    qedit_partcharge = guienv->addScrollBar(true, rect<s32>(320,195+3*THREEDEDIT_SPACING,2*QEDITWINDOWW/3-25,215+3*THREEDEDIT_SPACING),
                                            chargeditor, GUIQEPartCharge);
    qedit_partcharge->setSmallStep(1);
    qedit_partcharge->setLargeStep(2);
    qedit_partcharge->setMin(-1);
    qedit_partcharge->setMax(1);
    guienv->addStaticText(L"+", rect<s32>(2*QEDITWINDOWW/3-20,195+3*THREEDEDIT_SPACING,2*QEDITWINDOWW/3-5,215+3*THREEDEDIT_SPACING),
                          false, false, chargeditor,GUIText);

    guienv->addStaticText(L"Polarizability:",
                          rect<s32>(2 * (QEDITWINDOWW)/3,195+3*THREEDEDIT_SPACING,2 * (QEDITWINDOWW)/3+120,215+3*THREEDEDIT_SPACING),
                          false, true, chargeditor, GUIText);
    qedit_polariz = guienv->addEditBox(L"",
                                       rect<s32>(2 * (QEDITWINDOWW)/3+100,195+3*THREEDEDIT_SPACING,QEDITWINDOWW-20,215+3*THREEDEDIT_SPACING),
                                       true,chargeditor,GUIQEPolarizability);
    //qedit_polariz = guienv->addEditBox(L"", rect<>)

    qedit_move = guienv->addButton(core::rect<s32>(20, 225+3*THREEDEDIT_SPACING, 220, QEDITWINDOWH-20), chargeditor, GUIQEMove, L"Move Charge");

    qedit_clone = guienv->addButton(core::rect<s32>(QEDITWINDOWW/2 - 100, 225+3*THREEDEDIT_SPACING, QEDITWINDOWW/2+100, QEDITWINDOWH-20), chargeditor, GUIQEClone, L"Copy Charge");

    qedit_delete = guienv->addButton(core::rect<s32>(QEDITWINDOWW-220, 225+3*THREEDEDIT_SPACING, QEDITWINDOWW-20, QEDITWINDOWH-20), chargeditor, GUIQEDelete, L"Delete Charge");

    update_qecombobox();
    chargedit_select(0);
    qedit_cb->setSelected(0);

  }
  void update_chargedit();
  void chargedit_select(irr::s32 selection)
  {
    if(chargeditor != nullptr)
      {
        list<IGUIElement*> childlist = (chargeditor->getChildren());
        list<IGUIElement*>::Iterator it;
        if (selection > 0)
          {
            for (it = childlist.begin(); it != childlist.end(); ++it)
              {
                (*it)->setVisible(true);
              }
            chargeditor->getMinimizeButton()->setVisible(false);
            chargeditor->getMaximizeButton()->setVisible(false);
            chargeditor->setRelativePosition(rect<s32>(0,0,QEDITWINDOWW, QEDITWINDOWH)
                                             +chargeditor->getRelativePosition().UpperLeftCorner);
            qedit_cb->setSelected(selection);
            update_chargedit();
          }
        else
          {
            for (it = childlist.begin(); it != childlist.end(); ++it)
              {
                (*it)->setVisible(false);
              }
            chargeditor->getCloseButton()->setVisible(true);
            qedit_cb->setVisible(true);
            chargeditor->setRelativePosition(rect<s32>(0,0,QEDITWINDOWW, 55)
                                             +chargeditor->getRelativePosition().UpperLeftCorner);
            qedit_cb->setSelected(selection);
          }
      }
  }

  void create_qecolorpicker()
  {
    qedit_color = new irr::gui::IGUIColorPicker(guienv,
                                                guienv->addWindow(rect<s32>(0,0,275,180)+
                                                                  qedit_colorbutton->getAbsolutePosition().UpperLeftCorner,
                                                                  false, L"Color Picker", 0, GUIQEColor),
                                                GUIQEColor, true);
    qedit_color->setAdvenced(true);
    qedit_color->setRelativePosition(rect<s32>(0,20,275,160));
    qedit_color->setPickedColor(qedit_colorbuttonimg->getColor());
    qedit_color->setSelectedColor(qedit_colorbuttonimg->getColor());
    qedit_color->getExtendButton()->setVisible(false);
    //((IGUIWindow *) qedit_color->getParent())->setDrawTitlebar(false);
    //((IGUIWindow *) qedit_color->getParent())->getCloseButton()->setVisible(false);
  }
  irr::gui::IGUIEditBox *cameditpos[3], *cameditvel[3], *cameditacc[3],
    *cameditapos[3], *cameditavel[3], *cameditaacc[3],
    *cameditspacedit;
  irr::gui::IGUIScrollBar *cameditspacescroll;

  void create_cameditor()
  {
#define CAMEDITWINDOWW 660
    //Não, não lhe vou somar 6.
#define CAMEDITWINDOWH 420
    cameditor = guienv->addWindow(rect<s32>(0, 0, CAMEDITWINDOWW, CAMEDITWINDOWH), false, L"Change Camera...", 0, GUICamEditor);
    MAKE_3D_EDITS(L"Linear Position",20,cameditpos,GUICamPos,cameditor,CAMEDITWINDOWW);
    MAKE_3D_EDITS(L"Linear Velocity",30+THREEDEDIT_SPACING,cameditvel,GUICamVel,cameditor,CAMEDITWINDOWW);
    MAKE_3D_EDITS(L"Linear Acceleration",40+2*THREEDEDIT_SPACING,cameditacc,GUICamAcc,cameditor,CAMEDITWINDOWW);
    MAKE_3D_EDITS(L"Angular Position",50+3*THREEDEDIT_SPACING,cameditapos,GUICamAPos,cameditor,CAMEDITWINDOWW);
    MAKE_3D_EDITS(L"Angular Velocity",60+4*THREEDEDIT_SPACING,cameditavel,GUICamAVel,cameditor,CAMEDITWINDOWW);
    MAKE_3D_EDITS(L"Angular Acceleration",70+5*THREEDEDIT_SPACING,cameditaacc,GUICamAAcc,cameditor,CAMEDITWINDOWW);

    guienv->addStaticText(L"Spatial scale:", core::rect<s32>(20,80+6*THREEDEDIT_SPACING,200, 100+6*THREEDEDIT_SPACING), false, false, cameditor, GUIText);
    cameditspacescroll = guienv->addScrollBar(true,core::rect<s32>(150,80+6*THREEDEDIT_SPACING,CAMEDITWINDOWW-220, 100+6*THREEDEDIT_SPACING), cameditor, GUICamScrollSpace);
    cameditspacescroll->setMin(-10);
    cameditspacescroll->setMax(10010);
    cameditspacescroll->setSmallStep(10);
    cameditspacescroll->setLargeStep(100);
    cameditspacedit = guienv->addEditBox(L"",core::rect<s32>(CAMEDITWINDOWW-200,80+6*THREEDEDIT_SPACING,CAMEDITWINDOWW-20,100+6*THREEDEDIT_SPACING), true, cameditor, GUIEditSpace);

  }


  irr::gui::IGUIEditBox *simparedittimedit,
    *simpareditlowlim[3], *simparedituplim[3],
    *simpareditEExt[3], *simpareditBExt[3],
    *simpareditlightspeed, *simparedithistsize;
  irr::gui::IGUIButton *simpareditautosizehist;
  irr::gui::IGUIScrollBar *simparedittimescroll;
  irr::gui::IGUIButton *simpareditbgcolb,*simpareditbndcolb;
  irr::gui::IGUIImage *simpareditbgcimg, *simpareditbndcimg;
  irr::gui::IGUIColorPicker *simpareditbgcolor, *simpareditbndcolor;


  void update_simpareditor()
  {
    if(simpareditor != nullptr)
      {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstringstream s;
        s << std::setprecision(std::numeric_limits<FLType>::digits10) << limits[0];
        simpareditlowlim[0]->setText(s.str().c_str());
        s.str(std::wstring());
        s << std::setprecision(std::numeric_limits<FLType>::digits10) << limits[1];
        simpareditlowlim[1]->setText(s.str().c_str());
        s.str(std::wstring());
        s << std::setprecision(std::numeric_limits<FLType>::digits10) << limits[2];
        simpareditlowlim[2]->setText(s.str().c_str());
        s.str(std::wstring());
        s << std::setprecision(std::numeric_limits<FLType>::digits10) << limits[3];
        simparedituplim[0]->setText(s.str().c_str());
        s.str(std::wstring());
        s << std::setprecision(std::numeric_limits<FLType>::digits10) << limits[4];
        simparedituplim[1]->setText(s.str().c_str());
        s.str(std::wstring());
        s << std::setprecision(std::numeric_limits<FLType>::digits10) << limits[5];
        simparedituplim[2]->setText(s.str().c_str());
        s.str(std::wstring());

        simpareditEExt[0]->setText(converter.from_bytes(Eext[0]).c_str());
        simpareditEExt[1]->setText(converter.from_bytes(Eext[1]).c_str());
        simpareditEExt[2]->setText(converter.from_bytes(Eext[2]).c_str());
        simpareditBExt[0]->setText(converter.from_bytes(Bext[0]).c_str());
        simpareditBExt[1]->setText(converter.from_bytes(Bext[1]).c_str());
        simpareditBExt[2]->setText(converter.from_bytes(Bext[2]).c_str());

        s << std::setprecision(std::numeric_limits<FLType>::digits10) << std::fixed << lightspeed << std::scientific;
        simpareditlightspeed->setText(s.str().c_str());
        s.str(std::wstring());
        s << hist.get_numr(true);
        simparedithistsize->setText(s.str().c_str());
        ((u32 *) simpareditbgcimg->getImage()->lock())[0]=background.color;
        simpareditbgcimg->getImage()->unlock();
        simpareditbgcimg->setColor(background);
        ((u32 *) simpareditbndcimg->getImage()->lock())[0]=boundcolor.color;
        simpareditbndcimg->getImage()->unlock();
        simpareditbndcimg->setColor(boundcolor);
      }
  }

  void create_simpareditor()
  {
#define SIMPAREDITWINDOWW 660
#define SIMPAREDITWINDOWH 420
    simpareditor = guienv->addWindow(rect<s32>(0, 0, SIMPAREDITWINDOWW, SIMPAREDITWINDOWH), false, L"Simulation Parameters Editor", 0, GUISimparEditor);
    guienv->addStaticText(L"Simulation Area",rect<s32>(20,20,SIMPAREDITWINDOWW-20, 40),
                          false, true, simpareditor, GUIText          )->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT);
    MAKE_3D_EDITS(L"Lower Limits",50,simpareditlowlim, GUISimparLowLimits,simpareditor, SIMPAREDITWINDOWW);
    MAKE_3D_EDITS(L"Upper Limits",60+1*THREEDEDIT_SPACING,simparedituplim, GUISimparUpLimits,simpareditor, SIMPAREDITWINDOWW);
    MAKE_3D_EDITS(L"External Electric Field",80+2*THREEDEDIT_SPACING,simpareditEExt, GUISimparEExt,simpareditor, SIMPAREDITWINDOWW);
    MAKE_3D_EDITS(L"External Magnetic Field",90+3*THREEDEDIT_SPACING,simpareditBExt, GUISimparEExt,simpareditor, SIMPAREDITWINDOWW);

    guienv->addStaticText(L"Temporal scale:", core::rect<s32>(20,110+4*THREEDEDIT_SPACING,200, 130+4*THREEDEDIT_SPACING), false, false, simpareditor, GUIText);
    simparedittimescroll = guienv->addScrollBar(true,core::rect<s32>(150,110+4*THREEDEDIT_SPACING,SIMPAREDITWINDOWW-220, 130+4*THREEDEDIT_SPACING), simpareditor, GUISimparScrollTime);
    simparedittimescroll->setMin(-10);
    simparedittimescroll->setMax(10010);
    simparedittimescroll->setSmallStep(10);
    simparedittimescroll->setLargeStep(100);
    simparedittimedit = guienv->addEditBox(L"",core::rect<s32>(SIMPAREDITWINDOWW-200,110+4*THREEDEDIT_SPACING,SIMPAREDITWINDOWW-20,130+4*THREEDEDIT_SPACING), true, simpareditor, GUIEditTime);

    guienv->addStaticText(L"Speed of light:", core::rect<s32>(20,140+4*THREEDEDIT_SPACING,200, 160+4*THREEDEDIT_SPACING), false, false, simpareditor, GUIText);
    simpareditlightspeed = guienv->addEditBox(L"",core::rect<s32>(125,140+4*THREEDEDIT_SPACING,SIMPAREDITWINDOWW/2-10,160+4*THREEDEDIT_SPACING), true, simpareditor, GUISimparLightspeed);
    guienv->addStaticText(L"History size:", core::rect<s32>(SIMPAREDITWINDOWW/2,140+4*THREEDEDIT_SPACING,SIMPAREDITWINDOWW/2+200, 160+4*THREEDEDIT_SPACING), false, false, simpareditor, GUIText);
    simparedithistsize = guienv->addEditBox(L"",core::rect<s32>(SIMPAREDITWINDOWW/2+90,140+4*THREEDEDIT_SPACING,SIMPAREDITWINDOWW-130,160+4*THREEDEDIT_SPACING), true, simpareditor, GUISimparHistSize);
    simpareditautosizehist = guienv->addButton(core::rect<s32>(SIMPAREDITWINDOWW-120, 140+4*THREEDEDIT_SPACING, SIMPAREDITWINDOWW-20, 160+4*THREEDEDIT_SPACING), simpareditor, GUISimparAutosizeHist, L"Autosize");

    guienv->addStaticText(L"Background Colour:",core::rect<s32>(20,SIMPAREDITWINDOWH-40,SIMPAREDITWINDOWW/2, SIMPAREDITWINDOWH-20), false, false, simpareditor, GUIText);
    guienv->addStaticText(L"Bounds Colour:",core::rect<s32>(SIMPAREDITWINDOWW/2,SIMPAREDITWINDOWH-40,SIMPAREDITWINDOWW, SIMPAREDITWINDOWH-20), false, false, simpareditor, GUIText);


    simpareditbgcolb = guienv->addButton(rect<s32>(160,SIMPAREDITWINDOWH-40,210, SIMPAREDITWINDOWH-20), simpareditor, GUISimparBGColButton);
    simpareditbgcimg = guienv->addImage(core::rect<s32>(5,5,45,15),simpareditbgcolb,-1, L"",false);
    simpareditbgcimg->setImage(guienv->getVideoDriver()->addTexture({1, 1}, "backgroundcolor", video::ECF_A8R8G8B8));
    simpareditbgcimg->setScaleImage(true);

    simpareditbndcolb = guienv->addButton(rect<s32>(SIMPAREDITWINDOWW/2+110,SIMPAREDITWINDOWH-40,SIMPAREDITWINDOWW/2+160, SIMPAREDITWINDOWH-20), simpareditor, GUISimparBndColButton);
    simpareditbndcimg = guienv->addImage(core::rect<s32>(5,5,45,15),simpareditbndcolb,-1, L"",false);
    simpareditbndcimg->setImage(guienv->getVideoDriver()->addTexture({1, 1}, "backgroundcolor", video::ECF_A8R8G8B8));
    simpareditbndcimg->setScaleImage(true);

    update_simpareditor();
  }

  void create_backgroundcolorpicker()
  {
    simpareditbgcolor = new irr::gui::IGUIColorPicker(guienv,
                                                      guienv->addWindow(rect<s32>(0,0,275,180)+
                                                                        simpareditbgcolb->getAbsolutePosition().UpperLeftCorner,
                                                                        false, L"Color Picker", 0, GUISimparBGCol),
                                                      GUISimparBGCol, true);
    simpareditbgcolor->setAdvenced(true);
    simpareditbgcolor->setRelativePosition(rect<s32>(0,20,275,160));
    simpareditbgcolor->setPickedColor(simpareditbgcimg->getColor());
    simpareditbgcolor->setSelectedColor(simpareditbgcimg->getColor());
    simpareditbgcolor->getExtendButton()->setVisible(false);
  }

  void create_boundcolorpicker()
  {
    simpareditbndcolor = new irr::gui::IGUIColorPicker(guienv,
                                                       guienv->addWindow(rect<s32>(0,0,275,180)+
                                                                         simpareditbndcolb->getAbsolutePosition().UpperLeftCorner,
                                                                         false, L"Color Picker", 0, GUISimparBndCol),
                                                       GUISimparBndCol, true);
    simpareditbndcolor->setAdvenced(true);
    simpareditbndcolor->setRelativePosition(rect<s32>(0,20,275,160));
    simpareditbndcolor->setPickedColor(simpareditbndcimg->getColor());
    simpareditbndcolor->setSelectedColor(simpareditbndcimg->getColor());
    simpareditbndcolor->getExtendButton()->setVisible(false);
  }

  irr::gui::IGUIButton *keywindowkeys[TOTALKEYS];

  int expecting_keybinding;
  //Este parâmetro indica o tipo de tecla
  //que o utilizador quererá redefinir.

  void update_keywindow()
  {
    expecting_keybinding = -1;
    if(keywindow != nullptr)
      {
        size_t i;
        for(i = 0; i < TOTALKEYS; ++i)
          {
            keywindowkeys[i]->setText(key_to_name(keysettings[i]).c_str());
          }
      }
  }


  void create_keywindow()
  {
#define KEYWINDOWW 480
#define KEYWINDOWH 480
    keywindow = guienv->addWindow(rect<s32>(0, 0, KEYWINDOWW, KEYWINDOWH), false, L"Keybindings...", 0, GUIKeybinding);
    size_t i;
    for(i = 0; i < TOTALKEYS; ++i)
      {
        guienv->addStaticText(keytype_to_string(i).c_str(), rect<s32>(20,20+i*30,200,40+i*30),false, false, keywindow, GUIText);
        keywindowkeys[i] = guienv->addButton(rect<s32>(210,20+i*30,KEYWINDOWW-20,40+i*30),keywindow, GUIKey0+i);
        keywindowkeys[i]->setIsPushButton(true);
      }
    update_keywindow();
  }


  irr::gui::IGUIEditBox *recw_filepath, *recw_recordnum, *recw_recordint;
  irr::gui::IGUIButton *recw_selectfile;

  void update_recordwindow()
  {
    if(recordwindow != nullptr)
      {
        recw_filepath->setText(recsave.c_str());
        recw_recordnum->setText(std::to_wstring(totalprint).c_str());
        recw_recordint->setText(std::to_wstring(printerval).c_str());
      }
  }

  void create_recordwindow()
  {
#define RECWINDOWW 700
#define RECWINDOWH 100
    recordwindow = guienv->addWindow(rect<s32>(0, 0, RECWINDOWW, RECWINDOWH), false, L"Record Simulation...", 0, GUIRecordWindow);
    guienv->addStaticText(L"File: ", rect<s32>(20,20,100,40),false, false, recordwindow, GUIText);
    recw_filepath = guienv->addEditBox(L"", rect<s32>(70,20,RECWINDOWW-140,40),true, recordwindow, GUIRWFilePath);
    recw_selectfile = guienv->addButton(rect<s32>(RECWINDOWW-120,20,RECWINDOWW-20,40),recordwindow, GUIRWFileSelect, L"Select");

    guienv->addStaticText(L"Number of recordings:", core::rect<s32>(20, 60, 180, 80), false, false, recordwindow, GUIText);
    guienv->addStaticText(L"Frames per recording:", core::rect<s32>(RECWINDOWW/2+10, 60, RECWINDOWW/2+180, 80), false, false, recordwindow, GUIText);

    recw_recordnum = guienv->addEditBox(L"", core::rect<s32>(180, 60, RECWINDOWW/2-10, 80), true, recordwindow, GUIRecordNum);
    recw_recordint = guienv->addEditBox(L"", core::rect<s32>(RECWINDOWW/2+180, 60, RECWINDOWW-20, 80), true, recordwindow, GUIRecordInterval);
    update_recordwindow();
  }

  irr::gui::IGUIScrollBar *opt_ROCscroll, *opt_ITCscroll, *opt_threadscroll;
  irr::gui::IGUIEditBox *opt_ROCedit, *opt_ITCedit, *opt_threadedit;
  irr::gui::IGUICheckBox *opt_invx, *opt_invy, *opt_relmov;

  void update_optionwindow()
  {
    if(optionwindow != nullptr)
      {
        opt_invx->setChecked(invertx);
        opt_invy->setChecked(inverty);
        opt_relmov->setChecked(relative);
        opt_ITCedit->setText(std::to_wstring(itercount).c_str());
        opt_ITCscroll->setPos(itercount);
        opt_threadedit->setText(std::to_wstring(numthreads).c_str());
        opt_threadscroll->setPos(numthreads);
      }
  }

  void create_optionwindow()
  {
#define OPTWINDOWW 700
#define OPTWINDOWH 260
    optionwindow = guienv->addWindow(rect<s32>(0, 0, OPTWINDOWW, OPTWINDOWH), false, L"Options", 0, GUIOptions);
    ( guienv->addStaticText( L"Invert x-axis rotations?",
                             core::rect<s32>(20, 20, OPTWINDOWW, 50),
                             false, false, optionwindow, GUIText)                )->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

    opt_invx = guienv->addCheckBox( invertx, core::rect<s32>(OPTWINDOWW - 39, 20, OPTWINDOWW - 20, 50),
                                    optionwindow, GUIOptInvx                                         );

    ( guienv->addStaticText( L"Invert z-axis rotations?",
                             core::rect<s32>(20, 60, OPTWINDOWW, 90),
                             false, false, optionwindow, GUIText )               )->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

    opt_invy = guienv->addCheckBox( inverty, core::rect<s32>(OPTWINDOWW - 39, 60, OPTWINDOWW - 20, 90),
                                    optionwindow, GUIOptInvy                                            );

    ( guienv->addStaticText( L"Use relative movement?",
                             core::rect<s32>(20, 100, OPTWINDOWW, 130),
                             false, false, optionwindow, GUIText )               )->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

    opt_relmov = guienv->addCheckBox( relative, core::rect<s32>(OPTWINDOWW - 39, 100, OPTWINDOWW - 20, 130),
                                      optionwindow, GUIOptRelMov                                            );

    ( guienv->addStaticText( L"Rate of change:",
                             core::rect<s32>(20, 140, OPTWINDOWW, 170),
                             false, false, optionwindow, GUIText )       )->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

    opt_ROCscroll = guienv->addScrollBar(true, core::rect<s32>(200,145,500,165), optionwindow, GUIOptROCScroll);
    opt_ROCscroll->setSmallStep(10);
    opt_ROCscroll->setLargeStep(100);
    opt_ROCscroll->setMin(-10);
    opt_ROCscroll->setMax(10010);
    opt_ROCedit = guienv->addEditBox( L"", core::rect<s32>(520,145,OPTWINDOWW - 20,165),
                                      true, optionwindow, GUIOptROCEdit                 );

    ( guienv->addStaticText( L"Iterations per frame:",
                             core::rect<s32>(20, 180, OPTWINDOWW, 210),
                             false, false, optionwindow, GUIText)             )->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

    opt_ITCscroll = guienv->addScrollBar(true, core::rect<s32>(200,185,500,205), optionwindow, GUIOptITCScroll);
    opt_ITCscroll->setSmallStep(1);
    opt_ITCscroll->setLargeStep(5);
    opt_ITCscroll->setMin(1);
    opt_ITCscroll->setMax(1000);
    opt_ITCedit = guienv->addEditBox( L"", core::rect<s32>(520,185,OPTWINDOWW - 20,205),
                                      true, optionwindow, GUIOptITCEdit                 );


    ( guienv->addStaticText( L"Number of threads:",
                             core::rect<s32>(20, 220, OPTWINDOWW, 250),
                             false, false, optionwindow, GUIText)             )->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
    opt_threadscroll = guienv->addScrollBar(true, core::rect<s32>(200,225,500,245), optionwindow, GUIOptThreadScroll);
    opt_threadscroll->setSmallStep(1);
    opt_threadscroll->setLargeStep(2);
    opt_threadscroll->setMin(1);
    opt_threadscroll->setMax(omp_get_num_procs());
    opt_threadedit = guienv->addEditBox( L"", core::rect<s32>(520,225,OPTWINDOWW - 20,245),
                                         true, optionwindow, GUIOptThreadEdit                 );



    update_optionwindow();
  }
  void create_load_dialog()
  {
    opendialog = new CGUIFileSelector(L"Load configuration...", guienv, guienv->getRootGUIElement(),GUILoadDialog, CGUIFileSelector::EFST_OPEN_DIALOG);
    opendialog->setCustomFileIcon(driver->getTexture(L"Icons/file.png"));
    opendialog->setCustomDirectoryIcon(driver->getTexture(L"Icons/folder.png"));
    opendialog->addFileFilter(L"Configuration", L"sce", driver->getTexture(L"Icons/simcampel_small.png"));
    opendialog->addPlacePaths(L"Example",L"./Examples", driver->getTexture(L"Icons/places_folder.png"));
    opendialog->addPlacePaths(L"Custom",L"./Custom", driver->getTexture(L"Icons/places_folder.png"));
    opendialog->setDevice(device);
    opendialog->setStartingPath(L"./Examples");
    opendialog->setVisible(true);
  }
  void create_simple_load_dialog()
  {
    opendialog = new CGUIFileSelector(L"Load configuration without .aux...", guienv, guienv->getRootGUIElement(),GUISimpleLoadDialog, CGUIFileSelector::EFST_OPEN_DIALOG);
    opendialog->setCustomFileIcon(driver->getTexture(L"Icons/file.png"));
    opendialog->setCustomDirectoryIcon(driver->getTexture(L"Icons/folder.png"));
    opendialog->addFileFilter(L"Configuration", L"sce", driver->getTexture(L"Icons/simcampel_small.png"));
    opendialog->addPlacePaths(L"Example",L"./Examples", driver->getTexture(L"Icons/places_folder.png"));
    opendialog->addPlacePaths(L"Custom",L"./Custom", driver->getTexture(L"Icons/places_folder.png"));
    opendialog->setDevice(device);
    opendialog->setStartingPath(L"./Examples");
    opendialog->setVisible(true);
  }
  void create_save_dialog()
  {
    opendialog = new CGUIFileSelector(L"Save configuration...", guienv, guienv->getRootGUIElement(),GUISaveDialog, CGUIFileSelector::EFST_SAVE_DIALOG);
    opendialog->setCustomFileIcon(driver->getTexture(L"Icons/file.png"));
    opendialog->setCustomDirectoryIcon(driver->getTexture(L"Icons/folder.png"));
    opendialog->addFileFilter(L"Configuration", L"sce", driver->getTexture(L"Icons/simcampel_small.png"));
    opendialog->addPlacePaths(L"Example",L"./Examples", driver->getTexture(L"Icons/places_folder.png"));
    opendialog->addPlacePaths(L"Custom",L"./Custom", driver->getTexture(L"Icons/places_folder.png"));
    opendialog->setDevice(device);
    opendialog->setStartingPath(L"./Custom");
    opendialog->setVisible(true);
  }
  void create_optload_dialog()
  {
    opendialog = new CGUIFileSelector(L"Load settings...", guienv, guienv->getRootGUIElement(),GUIOptLoadDialog, CGUIFileSelector::EFST_OPEN_DIALOG);
    opendialog->setCustomFileIcon(driver->getTexture(L"Icons/file.png"));
    opendialog->setCustomDirectoryIcon(driver->getTexture(L"Icons/folder.png"));
    opendialog->addFileFilter(L"Settings files", L"scs", driver->getTexture(L"Icons/file.png"));
    opendialog->setDevice(device);
    opendialog->setStartingPath(L"./");
    opendialog->setVisible(true);
  }
  void create_optsave_dialog()
  {
    opendialog = new CGUIFileSelector(L"Save settings...", guienv, guienv->getRootGUIElement(),GUIOptSaveDialog, CGUIFileSelector::EFST_SAVE_DIALOG);
    opendialog->setCustomFileIcon(driver->getTexture(L"Icons/file.png"));
    opendialog->setCustomDirectoryIcon(driver->getTexture(L"Icons/folder.png"));
    opendialog->addFileFilter(L"Settings files", L"scs", driver->getTexture(L"Icons/file.png"));
    opendialog->setDevice(device);
    opendialog->setStartingPath(L"./");
    opendialog->setVisible(true);
  }
  void create_record_dialog()
  {
    opendialog = new CGUIFileSelector(L"Save data to file...", guienv, guienv->getRootGUIElement(),GUIDataSaveDialog, CGUIFileSelector::EFST_SAVE_DIALOG);
    opendialog->setCustomFileIcon(driver->getTexture(L"Icons/file.png"));
    opendialog->setCustomDirectoryIcon(driver->getTexture(L"Icons/folder.png"));
    opendialog->addFileFilter(L"Text files", L"txt", driver->getTexture(L"Icons/file.png"));
    opendialog->addFileFilter(L"Data files", L"dat", driver->getTexture(L"Icons/file.png"));
    opendialog->setDevice(device);
    opendialog->addPlacePaths(L"Recordings",L"./Recordings", driver->getTexture(L"Icons/places_folder.png"));
    opendialog->setStartingPath(L"./Recordings");
    opendialog->setVisible(true);
  }
  void create_mousemenu()
  {
    mousemenu = guienv->addContextMenu(rect<s32>(0,0,100,100), 0, GUIMouseMenu);
    mousemenu->addItem(L"Unidentified Charged Object", -1);
    //As trutas estarão lá fora?
    mousemenu->addSeparator();
    //p.mousemenu->addItem(L"Select Charge", MMenuSelect);
    mousemenu->addItem(L"Stop Charge", MMenuStop);
    mousemenu->addItem(L"Move Charge", MMenuMove);
    mousemenu->addSeparator();
    mousemenu->addItem(L"Clone Charge", MMenuClone);
    mousemenu->addSeparator();
    mousemenu->addItem(L"Edit Charge", MMenuEdit);
    mousemenu->addSeparator();
    mousemenu->addItem(L"Delete Charge", MMenuDelete);
    mousemenu->setCloseHandling(ECMC_REMOVE);
  }

  void show_lower_menu()
  {
    lowermenu->setVisible(true);
    list<IGUIElement*> childlist = (lowermenu->getChildren());
    list<IGUIElement*>::Iterator it;
    for (it = childlist.begin(); it != childlist.end(); ++it)
      {
        (*it)->setVisible(true);
      }
  }

  void hide_lower_menu()
  {
    list<IGUIElement*> childlist = (lowermenu->getChildren());
    list<IGUIElement*>::Iterator it;
    for (it = childlist.begin(); it != childlist.end(); ++it)
      {
        (*it)->setVisible(false);
      }
    lowermenu->setVisible(false);
  }
};

//Para o algoritmo de pesquisa do registo histórico
//(mais à frente)
#define LINEAR_SEARCH_THRESHOLD 4
//Faz-se uma pesquisa binária até o número de elementos ser
//inferior ou igual a LINEAR_SEARCH_THRESHOLD

#define MAXIMUM_HISTORY 1048576 //=2^20
//Para não fritar o computador...

size_t autosize_hist(const props &p)
{
  size_t i;
  FLType temp = 0;
  for(i = 0; i < 3; ++i)
    {
      temp += (p.limits[i] - p.limits[i+3])*(p.limits[i] - p.limits[i+3]);
    }
  return std::min<FLType>(std::ceil(sqrt(temp)/p.lightspeed/p.timescale*p.itercount*60)+LINEAR_SEARCH_THRESHOLD, MAXIMUM_HISTORY);
  //*60 por causa do VSync <=> 60fps.
}

void printhist(std::ostream &s, const props &p)
{
  size_t i, j;
  s << p.hist.get_numr(true) << " " << p.hist.get_numr() << "\n" << std::flush;
  for(i = 0; i < p.simarr.size(); ++i)
    {
      for(j = 0; j < p.hist.get_numr(); ++j)
        {
          s << p.hist(i,j) << " ";
        }
      s << "\n";
    }
}

bool readhist(std::istream &s, props &p)
{
  size_t i, j, k;
  p.hist.clear();
  s >> i >> j;
  if(!s.fail() && i != 0 && j != 0)
    {
      p.hist.resize_records(i);
      p.hist.add_element(p.simarr.size());
      for(i = 0; i < p.simarr.size(); ++i)
        {
          for(k = 0; k < j; ++k)
            {
              s >> p.hist(i,k,false);
              if(s.fail())
                {
                  return true;
                }
            }
        }
      return false;
    }
  else if(!s.fail() && i != 0)
    {
      p.hist.resize_records(i);
      p.hist.add_element(p.simarr.size());
      return false;
    }
  else
    {
      s.clear();
      p.hist.add_element(p.simarr.size());
      p.hist.resize_records(autosize_hist(p));
      return false;
    }
}

void reset_history_times(props &p, bool force_update = false)
{
  if(p.hist.get_numel() == 0)
    {
      return;
    }
  size_t i, j;
  if(p.hist.get_numr() == p.hist.get_numr(true) || force_update)
    {
      if(p.hist.get_newest_record(0).time/p.timetotal > std::numeric_limits<FLType>::epsilon() || force_update)
        {
          for (i = 0; i < p.simarr.size(); ++i)
            //Ciclo de actualização dos tempos no histórico...
            {
              for(j = 0; j < p.hist.get_numr(); ++j)
                {
                  p.hist(i,j,false).time -= p.timetotal;
                }
            }
          p.timetotal = 0;
        }
    }
}

FLType histr_comparison(const charge_rec & cr, const fspoint<FLType> pos, const FLType t, const FLType C)
//Retorna negativo se a distância percorrida pela luz
//for superior à distância entre as cargas
//(equivalente a o tempo que a luz demora
//a percorrer a distância entre as cargas
//ser inferior ao intervalo de tempo em causa).
//Comparação feita assim por, expectavelmente,
//a diferença de tempos ser muito mais pequena
//do que a de posições...
{
  return fabs(cr.pos - pos) - (t-cr.time)*C;
}
//NOTA: negativo nos mais antigos!


void linear_search_history(charge_rec &cr, const props &p, const fspoint<FLType> &pos, const FLType t, const size_t q, const size_t left, const size_t right)
{
  size_t i = left;
  FLType temp, pemt;
  temp = 0;
  pemt = histr_comparison(p.hist(q,left),pos,t,p.lightspeed);
  if(pemt > 0)
    //A distância percorrida pela luz é menor
    //do que a que separa as cargas
    {
      return;
    }
  for (i = left+1; i <= right; ++i)
    {
      if(std::isnan(p.hist(q,i).time))
        {
          return;
          //Daqui para a frente, só NaN...
        }
      temp = histr_comparison(p.hist(q,i),pos,t,p.lightspeed);
      if(temp == 0)
        {
          cr = p.hist(q,i);
          return;
        }
      else if(temp*pemt < 0)
        //Sinais contrários, logo, o que queremos está no meio.
        {
          cr = (p.hist(q,i-1)*pemt+p.hist(q,i)*temp)/(pemt+temp);
          return;
        }
    }
  if(temp < 0)
  {
    cr = p.hist(q,right);
  }
}

std::pair<fspoint<FLType>,fspoint<FLType>> calculate_fields(props &p, const fspoint<FLType> &pos, const FLType t, const size_t ignore = std::numeric_limits<size_t>::max())
{
  fspoint<FLType> n, beta, e(0.0), b(0.0);
  //Temporários.
  size_t i;
  //Iterar pelas cargas.
  size_t l, r, m;
  FLType temp;
  //Fazer a binary search.
  charge_rec cr;
  //Isto armazena temporariamente o registo relevante
  //para a posição e tempo considerados.

  FLType vars[] = {pos[0], pos[1], pos[2], t};
  //x, y, z e t
  //(para avaliação dos campos)

  fspoint<FLType> E_, B_;
  E_[0] = p.Efunc[0].Eval(vars);
  E_[1] = p.Efunc[1].Eval(vars);
  E_[2] = p.Efunc[2].Eval(vars);
  B_[0] = p.Bfunc[0].Eval(vars);
  B_[1] = p.Bfunc[1].Eval(vars);
  B_[2] = p.Bfunc[2].Eval(vars);

  if(p.hist.get_numel() < 1 || p.hist.get_numr() < 1)
    {
      return std::pair<fspoint<FLType>,fspoint<FLType>>(E_, B_);
    }
  for (i = 0; i < p.hist.get_numel(); ++i)
    {
      if (i == ignore)
        {
          continue;
        }
      cr.initialize();
      l = 0;
      r = p.hist.get_numr()-1;
      while(l <= r && r < p.hist.get_numr())
        {
          m = l/2+r/2;
          if(r-l <= LINEAR_SEARCH_THRESHOLD)
            {
              linear_search_history(cr, p, pos, t, i, l, r);
              break;
            }
          temp = histr_comparison(p.hist(i,m),pos,t,p.lightspeed);
          if(temp > 0)
            //Se a luz ainda não tiver percorrido a distância
            {
              r = m;
            }
          else if(temp < 0)
            //Se a luz já tiver percorrido a distância
            {
              l = m;
            }
          else //temp == 0
            {
              cr = p.hist(i,m);
              break;
            }
        }
      if(cr.initialized())
        {
          continue;
        }
      n = (pos-cr.pos).normalize();
      beta = cr.vel/p.lightspeed;

      //Temporário, para facilitar.
      beta = COULCONS__ * cr.q *pow(1-dotp(n, beta),-3) * (
                                                           (1.0-cr.vel.square_norm2()/p.lightspeed/p.lightspeed) * (n - beta)/(pos-cr.pos).square_norm2() +
                                                           crossp(n, crossp(n-beta, cr.acc/p.lightspeed))/p.lightspeed/fabs(pos-cr.pos)                    );
      e += beta;
      b += crossp(n/p.lightspeed,beta);
      //Conforme https://en.wikipedia.org/wiki/Li%C3%A9nard%E2%80%93Wiechert_potential
    }
  e += E_;
  b += B_;
  //Consideramos a possibilidade de haver
  //campos exteriormente aplicados...

  return std::pair<fspoint<FLType>,fspoint<FLType>>(e,b);

}


/***************************************

                CHARGE

****************************************/

struct charge
//Uma partícula com carga, de massa m e carga q.
{
  fspoint<FLType> pos, vel, acc;
  //Por uma questão de facilidade de tratamento das
  //colisões das partículas, armazenar-se-á a posição e a velocidade.
  //A resolução das equações do movimento (relativísticas)
  //necessita, igualmente, de um termo de aceleração, pelo que
  //este é, também, armazenado.
  std::wstring label;
  FLType m, q, r;

  irr::scene::IMeshSceneNode *node;
  irr::video::SColor col;

  bool deformable;

  bool vertex_changed;

  float polarizability;
  signed char part_charge;

  void graphinfo_init(props &p, irr::s32 idx = -1)
  {
    node = p.smgr->addSphereSceneNode(1.0, 64, 0, idx);
    if(!node)
      {
        std::cerr << "Error: unable to create a sphere node! Exiting now...\n" << std::endl;
        exit(1);
      }
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    p.smgr->getMeshManipulator()->setVertexColors(node->getMesh(), col);
  }


private:

  irr::core::vector3df polchange_both(const irr::core::vector3df & orig, const irr::core::vector3df & E, const float polar)
  {
    return orig + polar * orig.dotProduct(E/E.getLength()) * E;
  }

  irr::core::vector3df polchange_positive(const irr::core::vector3df & orig, const irr::core::vector3df & E, const float polar)
  {
    float temp = orig.dotProduct(E/E.getLength());
    if(temp <= 0)
      {
        return orig;
      }
    else
      {
        return orig + polar * fabs(temp) * E;
      }
  }

  irr::core::vector3df polchange_negative(const irr::core::vector3df & orig, const irr::core::vector3df & E, const float polar)
  {
    float temp = orig.dotProduct(E/E.getLength());
    if(temp >= 0)
      {
        return orig;
      }
    else
      {
        return orig - polar * fabs(temp) * E;
      }
  }


public:

  void update_vertex_positions(const irr::core::vector3df &E, const vector3df *orig_pos)
  {
    IMeshBuffer *buf = node->getMesh()->getMeshBuffer(0);
    //E=E.normalize();
    if(!deformable || polarizability == 0 || E.getLengthSQ() == 0)
      {
        if(vertex_changed)
          {
            for(u32 i = 0; i < buf->getVertexCount();++i)
              {
                buf->getPosition(i) = orig_pos[i];
              }
            node->setMaterialFlag(irr::video::EMF_WIREFRAME, false);
            node->getMesh()->setDirty();
            vertex_changed = false;
          }
        return;
      }
    if(part_charge > 0)
      {
        for(u32 i = 0; i < buf->getVertexCount();++i)
          {
            buf->getPosition(i) = polchange_positive(orig_pos[i], E, polarizability);
          }
      }
    else if(part_charge == 0)
      {
        for(u32 i = 0; i < buf->getVertexCount();++i)
          {
            buf->getPosition(i) = polchange_both(orig_pos[i], E, polarizability);
          }
      }
    else if(part_charge < 0)
      {
        for(u32 i = 0; i < buf->getVertexCount();++i)
          {
            buf->getPosition(i) = polchange_negative(orig_pos[i], E, polarizability);
          }
      }
    node->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
    node->getMesh()->setDirty();
    vertex_changed = true;
  }


  void draw(props &p, const fspoint<FLType> &Efield)
  //Prepara as cargas para representação tridimensional.
  //O Efield é usado, no caso de haver deformabilidade,
  //para alterar as posições dos nodos...
  {
    if(node != nullptr)
      {
        irr::core::matrix4 m;
        irr::core::vector3df v;
        FLType tempfl = vel.square_norm2();
        if(tempfl != 0)
          {
            fspoint<FLType> tempv = vel/sqrt(tempfl);
            v.X = tempv[0];
            v.Y = tempv[1];
            v.Z = tempv[2];
            m.buildRotateFromTo(core::vector3df(0,0,1), v);
            node->setRotation(m.getRotationDegrees());
            node->setScale(irr::core::vector3df(r / p.scale, r / p.scale, sqrt(1-tempfl/p.lightspeed/p.lightspeed)* r / p.scale));
            m.buildRotateFromTo(v,core::vector3df(0,0,1));
            v = core::vector3df(Efield[0],Efield[1],Efield[2]);
            m.rotateVect(v);
            update_vertex_positions(v, p.vertexpos);
          }
        else
          {
            node->setScale(irr::core::vector3df(r / p.scale, r / p.scale, r / p.scale));
            update_vertex_positions(core::vector3df(Efield[0],Efield[1],Efield[2]), p.vertexpos);
          }

        node->setPosition(irr::core::vector3df( ((float)pos[0])/p.scale,
                                                ((float)pos[1])/p.scale,
                                                ((float)pos[2])/p.scale) );
      }
  }

  void clear()
  //Para remover uma carga da visualização...
  {
    if(node != nullptr)
      {
        node->remove();
        node = nullptr;
      }
  }

  void graphic_output(std::ostream &s) const
  {
    s << col.color << " " << deformable << " " << part_charge << " " << polarizability;
  }

  void graphic_input(std::istream &s)
  {
    s >> col.color >> deformable >> part_charge >> polarizability;
  }

  charge(const std::wstring &str = L"", const bool random = false):
    pos(0.0), vel(0.0), acc(0.0), label(str)
  {
    //Para inicializar aleatoriamente a carga...
    if(random)
      {
        q = 10.0*((FLType) rand())/((FLType) RAND_MAX)-5.0;
        m = 5.0*((FLType) rand())/((FLType) RAND_MAX);
        r = ((FLType) rand())/((FLType) RAND_MAX)+0.01;
      }
    else
      {
        m = 1.0;
        r = 1.0;
        q = 0.0;
      }
    deformable = false;
    polarizability = 0.0f;
    part_charge = 0;
    vertex_changed = false;
    node = nullptr;
  }

  charge(const props &p, const std::wstring &str = L"", const bool random = false):
    vel(0.0), acc(0.0), label(str)
  {
    size_t i;
    //Para inicializar aleatoriamente a carga...
    m = 1.0;
    if(random)
      {
        r = p.scale*(((FLType)rand())/((FLType)RAND_MAX) + 0.5);
        q = 2.0*((FLType)rand())/((FLType)RAND_MAX) - 1.0;
        for(i = 0; i < 3; ++i)
          {
            pos[i] = p.limits[i] + r + (p.limits[i+3]-p.limits[i] - 2*r)*((FLType)rand())/((FLType)RAND_MAX);
          }
      }
    else
      {
        r = 1.0;
        pos[0] = 0.0;
        pos[1] = 0.0;
        pos[2] = 0.0;
        q = 0.0;
      }
    deformable = false;
    polarizability = 0.0f;
    part_charge = 0;
    vertex_changed = false;
    node = nullptr;
  }
  charge(const fspoint<FLType> &p, const std::wstring &str = L"", const bool random = false):
    pos(p), vel(0.0), acc(0.0), label(str)
  {
    if(random)
      {
        q = 10.0*((FLType) rand())/((FLType) RAND_MAX)-5.0;
        m = 5.0*((FLType) rand())/((FLType) RAND_MAX);
        r = ((FLType) rand())/((FLType) RAND_MAX)+0.01;
      }
    else
      {
        q = 0.0;
        m = 1.0;
        r = 1.0;
      }
    deformable = false;
    polarizability = 0.0f;
    part_charge = 0;
    vertex_changed = false;
    node = nullptr;
  }
  charge( const fspoint<FLType> &p, const fspoint <FLType> &v, const fspoint <FLType> &a,
          const std::wstring &str = L"", const bool random = false ):
    pos(p), vel(v), acc(a), label(str)
  {
    if(random)
      {
        q = 10.0*((FLType) rand())/((FLType) RAND_MAX)-5.0;
        m = 5.0*((FLType) rand())/((FLType) RAND_MAX);
        r = ((FLType) rand())/((FLType) RAND_MAX)+0.01;
      }
    else
      {
        q = 0.0;
        m = 1.0;
        r = 1.0;
      }
    deformable = false;
    polarizability = 0.0f;
    part_charge = 0;
    vertex_changed = false;
    node = nullptr;
  }
  charge( const fspoint<FLType> &p, const fspoint <FLType> &v, const fspoint<FLType> &a,
          const FLType mass, const FLType chrg, const FLType radius, const std::wstring &str = L"" ):
    pos(p), vel(v), acc(a), label(str), m(mass), r(radius), q(chrg)
  {
    deformable = false;
    polarizability = 0.0f;
    part_charge = 0.0f;
    vertex_changed = false;
    node = nullptr;
  }

  charge& operator= (const charge &c)
  {
    if(&c == this)
      {
        return (*this);
      }
    pos = c.pos;
    vel = c.vel;
    acc = c.acc;
    m = c.m;
    r = c.r;
    q = c.q;
    label = c.label;
    node = c.node;
    col = c.col;
    deformable = c.deformable;
    polarizability = c.polarizability;
    part_charge = c.part_charge;
    vertex_changed = c.vertex_changed;
    return(*this);
  }

  FLType radius_from_vector(const fspoint<FLType> &vect, const FLType lightspeed) const
  {
    FLType a = r*invgamma(lightspeed);
    return (a*r/sqrt((r*r-a*a)*cosangle(vel, vect)*cosangle(vel, vect)+a*a));
  }

  bool check_collision(const charge &c, const FLType Cc) const
  //Verifica se duas cargas estão a colidir.
  //Cc é a velocidade da luz.
  {
    if (&c == this)
      {
        return false;
      }
    fspoint<FLType> dist = pos - c.pos;
    if(fabs(dist) > r + c.r)
      {
        //Visto que o que se verifica é uma contracção do espaço,
        //se estiverem mais distantes do que a soma dos raios,
        //garantidamente que não estarão a colidir...
        //Isto permite poupar algumas contas...
        return false;
      }
    return fabs(dist) <= radius_from_vector(dist, Cc) + c.radius_from_vector(dist, Cc);

  }

  void collide(charge &c, const FLType Cc)
  //Efectua a colisão de duas cargas
  //(sem, no entanto, verificar se estão a colidir,
  //na eventualidade de se pretender uma colisão à distância...)
  {
    if (&c == this)
      {
        return;
      }
    fspoint<FLType> temp1(vel), temp2(pos - c.pos);
    //Para guardar temporariamente
    //a velocidade e a diferença de posições.
    vel = ((c.m*(2.0*c.vel - vel) + m*vel)/(m + c.m));
    c.vel = ((c.m*(2.0*temp1 - c.vel) + m*c.vel)/(m + c.m));
    temp1 = temp2.normalize()*(radius_from_vector(temp2, Cc)+c.radius_from_vector(temp2, Cc)-fabs(temp2))*0.5;
    pos += temp1;
    c.pos -= temp1;

    acc.set_all(0.0);
    c.acc.set_all(0.0);
    //Vamos admitir que, no instante imediatamente após a colisão,
    //as partículas ficam com aceleração nula.
  }

  bool process_collision(charge &c, const FLType Cc)
  //Trata de verificar se ocorre colisão
  //e, em caso afirmativo, faz as alterações
  //apropriadas...
  {
    if(check_collision(c, Cc))
      {
        collide(c, Cc);
        return true;
      }
    return false;
  }


  fspoint<FLType> p_to_v(const fspoint<FLType> &p, const props &prop) const
  //Transforma um momento em velocidade.
  {
    return p*prop.lightspeed/sqrt(m*m*prop.lightspeed*prop.lightspeed + p.square_norm2());
  }

  fspoint<FLType> p_to_v(const fspoint<FLType> &p, const FLType CC) const
  //Transforma um momento em velocidade.
  {
    return p*CC/sqrt(m*m*CC*CC + p.square_norm2());
  }

  fspoint<FLType> v_to_p(const props &p) const
  //Transforma a velocidade da partícula em momento.
  {
    return v_to_p(vel, p);
  }

  fspoint<FLType> v_to_p(const FLType lightspeed) const
  //Transforma a velocidade da partícula em momento.
  {
    return v_to_p(vel, lightspeed);
  }

  fspoint<FLType> v_to_p(const fspoint<FLType> &v, const props &p) const
  //Transforma uma velocidade em momento,
  //tendo em conta as propriedades da partícula.
  {
    return m*v/sqrt(1.0-v.square_norm2()/p.lightspeed/p.lightspeed);
  }

  fspoint<FLType> v_to_p(const fspoint<FLType> &v, const FLType lightspeed) const
  //Transforma uma velocidade em momento,
  //tendo em conta as propriedades da partícula.
  {
    return m*v/sqrt(1.0-v.square_norm2()/lightspeed/lightspeed);
  }

  FLType gamma(const props &p) const
  {
    return 1/sqrt(1-vel.square_norm2()/p.lightspeed/p.lightspeed);
  }

  FLType invgamma(const props &p) const
  {
    return sqrt(1-vel.square_norm2()/p.lightspeed/p.lightspeed);
  }

  FLType gamma(const FLType Cc) const
  {
    return 1/sqrt(1-vel.square_norm2()/Cc/Cc);
  }

  FLType invgamma(const FLType Cc) const
  {
    return sqrt(1-vel.square_norm2()/Cc/Cc);
  }

  fspoint <FLType> acceleration(const fspoint <FLType> &E, const fspoint <FLType> &B, const props &p) const
  {
    return (q ? (E+crossp(vel,B))*invgamma(p.lightspeed)*q/m : fspoint<FLType>(0.0));
  }

  fspoint <FLType> acceleration(const std::pair<fspoint<FLType>,fspoint<FLType>> &fs, const props &p) const
  {
    return (q ? (fs.first+crossp(vel,fs.second))*invgamma(p.lightspeed)*q/m : fspoint<FLType>(0.0));
  }

  void stop(const FLType dt)
  //dt é o intervalo de tempo durante o qual
  //se considera que se dá a paragem
  {
    acc = -vel/dt;
    vel.set_all(0);
  }

  ~charge()
  {
  }

  void reset_position(const props &p)
  //Põe a carga numa posição default
  //e em que não colida com outras,
  //algo útil para quando se queira criar uma nova carga
  //no decurso da simulação.
  {
    size_t i, j;
    pos.set_all(0);
    //Pôr no centro.
    FLType nums[3], norm;
    for(i = 0; i < p.simarr.size(); ++i)
      {
        //Ciclo de verificação de colisões com outras cargas.
        if(&p.simarr[i] == this)
          {
            //Exceptuando a própria, claro.
            continue;
          }
        if(fabs(p.simarr[i].pos-pos) <= p.simarr[i].r+r)
          //Se colidirem,
          {
            //Geram-se uns aleatórios para bater certo
            for(j = 0; j < 3; ++j)
              {
                nums[j] = 2*((FLType) rand())/((FLType) RAND_MAX)-1;
              }
            norm = sqrt(nums[0]*nums[0]+nums[1]*nums[1]+nums[2]*nums[2]);
            //Afasta-se da posição da carga colidente
            for (j = 0; j < 3; ++j)
              {
                pos[j] = p.simarr[i].pos[j]+(r+p.simarr[i].r)*nums[j]/norm;
              }
            i = 0;
            //Volta-se a verificar tudo desde início,
            //para garantir que não colide com nenhuma na nova posição...
          }
      }
  }
};

#define LIFT_NAN(x) ( std::isnan(x) ? std::numeric_limits<FLType>::max() : (x))
#define RAW_PRINT(vec)    LIFT_NAN(vec[0]) << " "       \
  << LIFT_NAN(vec[1]) << " "                            \
  << LIFT_NAN(vec[2])

std::ostream& operator<< (std::ostream &s, const charge &p)
//Faz o output de uma carga
//(para um ficheiro)
{
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

  s << RAW_PRINT(p.pos) << " " << RAW_PRINT(p.vel) << " " << RAW_PRINT(p.acc) << " "
    << p.m << " " << p.q << " " << p.r << " " << converter.to_bytes(p.label) << "\n";
  return s;
  //Tem de haver quebra de linha para que label contenha espaços...
}
#undef LIFT_NAN
#undef RAW_PRINT

std::istream& operator>> (std::istream &s, charge &p)
//Faz o input de uma carga
//(de um ficheiro)
{
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::string temp;
  p.pos.raw_read(s);
  p.vel.raw_read(s);
  p.acc.raw_read(s);
  s >> p.m >> p.q >> p.r >> std::ws;
  getline(s,temp);
  p.label = converter.from_bytes(temp);
  return s;
}

//Isto tem de estar aqui para se conseguir fazer este construtor...
charge_rec::charge_rec(const charge &c)
{
  pos = c.pos,
    vel = c.vel;
  acc = c.acc;
  time = std::nan("");
  m = c.m;
  q = c.q;
}


void props::change_selected(const irr::s32 new_selected)
{
  int i;
  if(new_selected == selected || (new_selected >= GUIArrow1 && new_selected <= GUIArrow6))
    {
      return;
    }
  if(toolinuse == ToolbarMove - ToolbarMove && selected > -1)
    {
      for(i = 0; i < 6; ++i)
        {
          arrow[i]->setVisible(false);
        }
    }
  selected = new_selected;
  if(toolinuse == ToolbarMove - ToolbarMove && selected > -1)
    {
      for(i = 0; i < 6; ++i)
        {
          arrow[i]->setVisible(true);
        }
    }
}

void props::delete_charge(const size_t index)
{
  simarr[index].clear();
  if(index != simarr.size() - 1)
    {
      simarr[index] = simarr.back();
      simarr[index].node->setID(index);
      temp_earr[index] = temp_earr.back();
    }
  simarr.pop_back();
  temp_earr.pop_back();
  hist.remove_element(index, true);
  //Usa o mesmo raciocínio do que aqui.
  if (chargeditor != nullptr)
    {
      update_qecombobox();
    }
}

void props::add_charge(const charge& q)
{
  simarr.push_back(q);
  simarr.back().graphinfo_init((*this), simarr.size()-1);
  hist.add_element();
  if(chargeditor != nullptr)
    {
      qedit_cb->addItem(simarr.back().label.c_str());
    }
  temp_earr.push_back(fspoint<FLType>(0.0));

}


/**********************************

 CONFIGURAÇÕES DE CARGAS E AFINS

***********************************/

#define CONFIG_EXT ".sce"
//A extensão a dar às configurações de cargas

bool save_simple_config(const char *name, const props &p)
//Guarda a configuração.
{
  std::string s(name);
  if( s[s.length()-1] != CONFIG_EXT[3] || s[s.length()-2] != CONFIG_EXT[2] ||
      s[s.length()-3] != CONFIG_EXT[1] || s[s.length()-4] != CONFIG_EXT[0]    )
    {
      s += CONFIG_EXT;
    }
  std::ofstream o(s);
  if (!o.is_open())
    {
      std::cerr << "Error: unable to open file '" << s <<"' for saving configuration.\n" << std::endl;
      return true;
    }
  o << std::setprecision(std::numeric_limits<FLType>::digits);
  size_t i;
  o << p.simarr.size() << "\n";
  for(i = 0; i < p.simarr.size(); ++i)
    {
      o << p.simarr[i]  << " ";
    }
  for(i = 0; i < 6; ++i)
    {
      o << p.limits[i] << " ";
    }
  o << "\n";
  for (i = 0; i < 3; ++i)
    {
      o << p.Eext[i] << "\n";
    }
  for (i = 0; i < 3; ++i)
    {
      o << p.Bext[i] << "\n";
    }
  o << p.lightspeed << " ";
  printhist(o, p);
  o << std::flush;
  if(o.fail())
    {
      o.close();
      std::cerr << "Error: unable to save configuration data in file'" << s <<"'\n" << std::endl;
      return true;
    }
  o.close();
  return false;
}

void generate_default_config(props &prop)
//Função que gera uma configuração-"padrão" das cargas.
{
  prop.simarr.resize(1);
  fspoint<FLType> p, v, a(0.0);
  p[0] = 0;
  p[1] = 0.0;
  p[2] = 0.0;
  v[0] = 0;
  v[1] = 0.0;
  v[2] = 0.0;
  prop.simarr[0] = charge(p, v, a, 1.0, 1.0, 1.0, L"Charge #1");
  prop.limits[0] = -20.0;
  prop.limits[1] = -20.0;
  prop.limits[2] = -20.0;
  prop.limits[3] = 20.0;
  prop.limits[4] = 20.0;
  prop.limits[5] = 20.0;
  prop.Eext[0] = "0";
  prop.Eext[1] = "0";
  prop.Eext[2] = "0";
  prop.Bext[0] = "0";
  prop.Bext[1] = "0";
  prop.Bext[2] = "0";
  prop.Efunc[0].Parse(prop.Eext[0],"x,y,z,t");
  prop.Efunc[1].Parse(prop.Eext[1],"x,y,z,t");
  prop.Efunc[2].Parse(prop.Eext[2],"x,y,z,t");
  prop.Bfunc[0].Parse(prop.Bext[0],"x,y,z,t");
  prop.Bfunc[1].Parse(prop.Bext[0],"x,y,z,t");
  prop.Bfunc[2].Parse(prop.Bext[0],"x,y,z,t");
  prop.timetotal = 0;
  prop.lightspeed = 299792458.0;
  prop.hist.clear();
  prop.hist.resize_records(4);
  prop.hist.add_element(prop.simarr.size());
  prop.temp_earr.resize(prop.simarr.size());
}

bool load_simple_config(std::ifstream &s, props &p)
//Lê uma configuração de um ficheiro.
{
  if (!s.is_open())
    {
      return true;
    }
  if(s.peek() == 0xEF)
  //UTF-8-BOM
  {
    s.ignore(3);
  }
  size_t i, num;
  s >> num;
  if(s.fail())
    {
      std::cerr << "Error: unable to determine the number of charges in the simulation.\n" << std::endl;
      return true;
    }
  p.simarr.resize(num);
  for(i = 0; i < num; ++i)
    {
      s >> p.simarr[i];
      if(s.fail())
        {
          std::cerr << "Error: unable to load charge #" << i+1 << ".\n" << std::endl;
          return true;
        }
      //Prefiramos a segurança à eficiência, neste caso...
    }
  for(i = 0; i < 6; ++i)
    {
      s >> p.limits[i];
    }
  if(s.fail())
    {
      std::cerr << "Error: unable to determine the limits of the simulation area.\n" << std::endl;
      return true;
    }
  s.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for(i = 0; i < 3; ++i)
    {
      getline(s,p.Eext[i]);
      p.Efunc[i].Parse(p.Eext[i],"x,y,z,t");
      p.Efunc[i].Optimize();
    }
  for(i = 0; i < 3; ++i)
    {
      getline(s,p.Bext[i]);
      p.Bfunc[i].Parse(p.Bext[i],"x,y,z,t");
      p.Bfunc[i].Optimize();
    }
  s >> p.lightspeed;
  if(readhist(s, p))
    {
      std::cerr << "Error: unable to properly load or create history.\n" << std::endl;
      return true;
    }
  p.temp_earr.resize(p.simarr.size());
  p.timetotal = 0;
  return false;
}


#define LAST_CONFIG "last" CONFIG_EXT
#define DEFAULT_CONFIG "default" CONFIG_EXT
//Para configurações anteriores de cargas.

void load_config(const char* name, props &p)
//Tenta ler a configuração, mas recorre às predefinições
//se a leitura não for bem-sucedida.
{
  std::ifstream f(name);
  if(!load_simple_config(f, p))
    {
      f.close();
      return;
    }
  f.close();
  if(std::strcmp(name, DEFAULT_CONFIG) != 0)
    {
      if(std::strcmp(name, LAST_CONFIG) != 0)
        {
          std::cerr << "Unable to load configuration from file '" << name
                    << "'. Attempting to load last configuration.\n" << std::endl;
          f.open(LAST_CONFIG);
          if(!load_simple_config(f, p))
            {
              f.close();
              return;
            }
          f.close();
        }
      std::cerr << "Unable to load last configuration. Attempting to load default configuration.\n" << std::endl;
      f.open(DEFAULT_CONFIG);
      if(!load_simple_config(f, p))
        {
          f.close();
          return;
        }
      f.close();
    }
  std::cerr << "Unable to load default configuration. Generating hard-coded default configuration.\n" << std::endl;
  generate_default_config(p);
}


/**********************************

 CONFIGURAÇÕES AUXILIARES

***********************************/

//Overloads para permitir o imput e output de alguns tipos da biblioteca usada.

std::ostream& operator<< (std::ostream &s, const vector3df &v)
{
  s << " " << v.X << " " << v.Y << " " << v.Z << " ";
  return s;
}

std::istream& operator>> (std::istream &s, vector3df &v)
{
  s >> v.X >> v.Y >> v.Z;
  return s;
}

std::ostream& operator<< (std::ostream &s, const quaternion &q)
{
  s << " " << q.X << " " << q.Y << " " << q.Z << " " << q.W << " ";
  return s;
}

std::istream& operator>> (std::istream &s, quaternion &q)
{
  s >> q.X >> q.Y >> q.Z >> q.W;
  return s;
}

void generate_aux_config(props &p)
//Gera as informações auxiliares para as cargas,
//com base nas informações principais da simulação.
{
  size_t j;
  p.change_selected(-1);
  for (j = 0; j < p.simarr.size(); ++j)
    {
      p.simarr[j].clear();
    }
#if ALTERNATE_COLOR_GEN
  FLType maxq = 0.0, maxm = 0.0;
  for (j = 0; j < p.simarr.size(); ++j)
    {
      maxq = std::max(maxq, fabs(p.simarr[j].q));
      maxm = std::max(maxm, p.simarr[j].m);
    }
  for (j = 0; j < p.simarr.size(); ++j)
    {
      p.simarr[j].col = SColor (255, ((unsigned int) (255.0 * p.simarr[j].q / maxq )) *
                                (p.simarr[j].q > 0) + (p.simarr[j].q < 0) * (rand() % 50),
                                ((unsigned int) (255.0 * p.simarr[j].m / maxm )),
                                ((unsigned int) (255.0 * p.simarr[j].q / maxq )) *
                                (p.simarr[j].q < 0) + (p.simarr[j].q > 0) * (rand() % 50));
      //Dado que a cor é apenas uma questão estética,
      //não é relevante a pouca aleatoriedade
      //dos dígitos menos significativos do gerador...
      p.simarr[j].graphinfo_init(p, j);
      p.simarr[j].deformable = false;
      p.simarr[j].part_charge = 0;
      p.simarr[j].polarizability = 0.0f;
    }
#else
  for (j = 0; j < p.simarr.size(); ++j)
    {
      p.simarr[j].col = irr::video::SColor (255, (p.simarr[j].q > 0) * (rand() % 200) + (p.simarr[j].q < 0) * (rand() % 25),
                                            (rand() % 100),
                                            (p.simarr[j].q < 0) * (rand() % 200) + (p.simarr[j].q > 0) * (rand() % 25));
      //Dado que a cor é apenas uma questão estética,
      //não é relevante a pouca aleatoriedade
      //dos dígitos menos significativos do gerador...
      p.simarr[j].graphinfo_init(p, j);
      p.simarr[j].deformable = false;
      p.simarr[j].part_charge = 0;
      p.simarr[j].polarizability = 0.0f;
    }

#endif
  p.background = irr::video::SColor(255, 245, 245, 245);
  p.boundcolor = irr::video::SColor(150, 150, 150, 150);
  p.c_lpos = p.c_lvel = p.c_lacc = p.c_avel = p.c_aacc = vector3df(0,0,0);
  p.lookatv.set(0,0,1);
  p.upv.set(0,1,0);
  p.sidev.set(1,0,0);
  p.scale = 1.0f;
  p.timescale = 1.0;
  p.rot.set(0,0,0);
}

#define AUXCONFIG_EXT ".aux"
//A extensão a adicionar ao ficheiro que corresponde aos dados auxiliares
//(para a parte de simulação gráfica)

bool save_aux_config(const char* name, const props &p)
//Guarda as informações auxiliares.
{
  std::string s(name);
  if( s[s.length()-1] != AUXCONFIG_EXT[3] || s[s.length()-2] != AUXCONFIG_EXT[2] ||
      s[s.length()-3] != AUXCONFIG_EXT[1] || s[s.length()-4] != AUXCONFIG_EXT[0]    )
    {
      s += AUXCONFIG_EXT;
    }
  std::ofstream o(s);
  if (!o.is_open())
    {
      std::cerr << "Error: unable to open file '" << s <<"' for saving auxiliary information.\n" << std::endl;
      return true;
    }
  size_t i;
  for(i = 0; i < p.simarr.size(); ++i)
    {
      p.simarr[i].graphic_output(o);
      o << " ";
    }
  o << p.background.color << " " << p.boundcolor.color << " "
    << p.c_lpos << " " << p.c_lvel
    << " " << p.c_lacc << " "  << " " << p.c_avel <<  " " << p.c_aacc
    << " " << p.lookatv << " " << p.upv << " " << p.sidev << " "
    << p.scale << " " << " " << p.timescale << p.rot;
  o << std::flush;
  if(o.fail())
    {
      std::cerr << "Error: unable to save all auxiliary information in file'" << s <<"'.\n" << std::endl;
      o.close();
      return true;
    }
  o.close();
  return false;
}

bool load_simple_aux_config(std::ifstream &s, props &p)
//Lê uma configuração auxiliar de um ficheiro.
{
  if (!s.is_open())
    {
      return true;
    }
  size_t i;
  for(i = 0; i < p.simarr.size(); ++i)
    {
      p.simarr[i].graphic_input(s);
      p.simarr[i].graphinfo_init(p,i);
      if(s.fail())
        {
          std::cerr << "Error: unable to load auxiliary information on charge #" << i+1 << ".\n" << std::endl;
          return true;
        }
      //Prefiramos a segurança à eficiência, neste caso...
    }
  s >> p.background.color >> p.boundcolor.color >> p.c_lpos >> p.c_lvel
    >> p.c_lacc >> p.c_avel >> p.c_aacc >> p.lookatv
    >> p.upv >> p.sidev >> p.scale >> p.timescale >> p.rot;
  if(s.fail())
    {
      std::cerr << "Error: unable to load general auxiliary information.\n" << std::endl;
      return true;
    }
  p.change_selected(-1);
  return false;
}

void load_aux_config(const char* name, props &p)
//Lê uma configuração auxiliar de um ficheiro.
//Caso falhe, gera as informações.
{
  std::string s(name);
  s += AUXCONFIG_EXT;
  std::ifstream f(s);
  if(!load_simple_aux_config(f, p))
    {
      f.close();
      p.play = false;
      return;
    }
  f.close();
  std::cerr << "Unable to load auxiliary information. Dynamically generating a set of suitable parameters.\n" << std::endl;
  generate_aux_config(p);
  p.play = false;
}

void full_load_config(const char* name, props &p)
//Lê as configurações, normais e auxiliares, com os habituais cuidados...
{
  std::ifstream f(name);
  p.changed = false;
  size_t i;
  p.change_selected(-1);
  for(i = 0; i < p.simarr.size(); ++i)
    {
      p.simarr[i].clear();
    }
  if(!load_simple_config(f, p))
    {
      f.close();
      load_aux_config(name, p);
      p.loaded = name;
      return;
    }
  f.close();
  p.loaded.clear();
  if(std::strcmp(name, DEFAULT_CONFIG) != 0)
    {
      if(std::strcmp(name, LAST_CONFIG) != 0)
        {
          std::cerr << "Unable to load configuration from file '" << name
                    << "'. Attempting to load last configuration.\n" << std::endl;
          f.open(LAST_CONFIG);
          if(!load_simple_config(f, p))
            {
              f.close();
              load_aux_config(LAST_CONFIG, p);
              return;
            }
          f.close();
        }
      std::cerr << "Unable to load last configuration. Attempting to load default configuration.\n" << std::endl;
      f.open(DEFAULT_CONFIG);
      if(!load_simple_config(f, p))
        {
          f.close();
          load_aux_config(DEFAULT_CONFIG, p);
          return;
        }
      f.close();
    }
  std::cerr << "Unable to load default configuration. Generating hard-coded default configuration.\n" << std::endl;
  generate_default_config(p);
  generate_aux_config(p);
}

void full_save_config(const char* name, const props &p)
//Salva uma configuração e as correspondentes informações auxiliares.
{
  std::string s(name);
  if( s[s.length()-1] != CONFIG_EXT[3] || s[s.length()-2] != CONFIG_EXT[2] ||
      s[s.length()-3] != CONFIG_EXT[1] || s[s.length()-4] != CONFIG_EXT[0]    )
    {
      s += CONFIG_EXT;
    }
  save_simple_config(s.c_str(), p);
  save_aux_config(s.c_str(), p);
}

#define SETTINGS_EXT ".scs"
//A extensão que indicia que se está perante um ficheiro de definições...

bool save_settings(const char* name, const props &p)
//Salva as definições num ficheiro.
{
  size_t i;
  std::string s(name);
  if( s[s.length()-1] != SETTINGS_EXT[3] || s[s.length()-2] != SETTINGS_EXT[2] ||
      s[s.length()-3] != SETTINGS_EXT[1] || s[s.length()-4] != SETTINGS_EXT[0]    )
    {
      s += SETTINGS_EXT;
    }
  std::ofstream f(s);
  if (!f.is_open())
    {
      std::cerr << "Error: unable to open file '" << s <<"' for saving settings.\n" << std::endl;
      return true;
    }
  f << p.invertx << " " << p.inverty << " " << p.relative << " "
    << p.rate_of_change << " " << p.itercount << " " << p.numthreads << " ";
  for(i = 0; i < TOTALKEYS; ++i)
    {
      f << p.keysettings[i] << " ";
    }
  if(f.fail())
    {
      f.close();
      std::cerr << "Error: unable to save all settings in file '" << s <<"'.\n" << std::endl;
      return true;
    }
  f.close();
  return false;
}

void make_default_settings(props &p)
//Gera as definições-padrão.
{
  p.rate_of_change = 2.0f;
  p.invertx = false;
  p.inverty = false;
  p.relative = true;
  p.itercount = 2;
  p.numthreads = 1;
  omp_set_num_threads(p.numthreads);
#define MAKEKEY(x,y) p.keysettings[x]=y
  //Para facilitar a atribuição das teclas...
  MAKEKEY(Forward, KEY_KEY_W); //W
  MAKEKEY(Backward, KEY_KEY_S); //S
  MAKEKEY(Left, KEY_KEY_A); //A
  MAKEKEY(Right, KEY_KEY_D); //D
  MAKEKEY(Up, KEY_KEY_Q); //Q
  MAKEKEY(Down, KEY_KEY_E); //E
  MAKEKEY(Velocity, KEY_LCONTROL); //Control (esquerdo)
  MAKEKEY(Acceleration, KEY_LSHIFT); //Shift (esquerdo)
  MAKEKEY(Angle, KEY_LMENU); //Alt (esquerdo)
  MAKEKEY(Stop, KEY_KEY_Z); //Z
  MAKEKEY(Reset, KEY_BACK); //Backspace
  MAKEKEY(BackToOrigin, KEY_HOME); //Home
  MAKEKEY(PlayPause, KEY_SPACE); //Barra de espaços;
  MAKEKEY(RelativeSwitch, KEY_KEY_P); //P
  MAKEKEY(OneStep, KEY_RETURN); //Enter
#undef MAKEKEY
}

std::ifstream& operator>> (std::ifstream &s, EKEY_CODE &c)
//Overload necessário para poder ler as definições das teclas.
{
  unsigned int a;
  s >> a;
  c = (EKEY_CODE) (a % (unsigned int) KEY_KEY_CODES_COUNT);
  return s;
}

bool load_simple_settings(std::ifstream &f, props &p)
//Lê as definições de um ficheiro.
{
  size_t i;
  if (!f.is_open())
    {
      return true;
    }
  f >> p.invertx >> p.inverty >> p.relative >> p.rate_of_change >> p.itercount >> p.numthreads;
  p.numthreads = std::min(p.numthreads, (size_t) omp_get_num_procs());
  omp_set_num_threads(p.numthreads);
  for(i = 0; i < TOTALKEYS; ++i)
    {
      f >> p.keysettings[i];
    }
  if(f.fail())
    {
      return true;
    }
  return false;
}

#define DEFAULT_SETTINGS "default" SETTINGS_EXT

void load_settings(const char* name, props &p)
//Tenta ler as definições; caso não consiga,
//lê as configurações-padrão; caso também não consiga,
//gera-as com base no que foi acima definido.
{
  std::ifstream f(name);
  p.changedsettings = false;
  if(!load_simple_settings(f, p))
    {
      p.loadedsettings = name;
      f.close();
      return;
    }
  f.close();
  p.loadedsettings.clear();
  if(std::strcmp(name, DEFAULT_SETTINGS) != 0)
    {
      std::cerr << "Error: unable to load settings from file '" << name << "'. Using default settings.\n" << std::endl;
      f.open(DEFAULT_SETTINGS);
      if(!load_simple_settings(f, p))
        {
          f.close();
          return;
        }
      f.close();
    }
  std::cerr << "Error: unable to load default settings. Using hard-coded settings.\n" << std::endl;
  make_default_settings(p);
  return;
}

bool write_data(std::ofstream &o, const std::vector<charge> &v, const FLType time, const FLType lightspeed)
//Faz o output das informações para o ficheiro para os gráficos.
{
  size_t i;
  if(!o.is_open())
    {
      return true;
    }
  o << time << "\n";
  for (i = 0; i < v.size(); ++i)
    {
      v[i].pos.raw_print(o);
      v[i].vel.raw_print(o);
      v[i].v_to_p(lightspeed).raw_print(o);
      o << "\n";
      if (o.fail())
        {
          return true;
        }
    }
  o << std::flush;
  return false;
}


/**********************************

 EVENT RECEIVER E GUI

***********************************/

void props::update_qecombobox()
{
  s32 i, temps32;
  if(chargeditor != nullptr)
    {
      temps32 = qedit_cb->getSelected();
      for(i = qedit_cb->getItemCount(); i > 0; --i)
        {
          qedit_cb->removeItem(i);
        }
      qedit_cb->removeItem(i);
      qedit_cb->addItem(L"");
      for(i = 0; i < simarr.size(); ++i)
        {
          qedit_cb->addItem(simarr[i].label.c_str());
        }
      qedit_cb->setSelected(temps32);
    }
}

#define UPDATE_UNFOCUSED(Element, Function) if(!guienv->hasFocus(Element)) { (Element)->Function; }
void props::update_chargedit()
{
  if(chargeditor != nullptr && qedit_cb->getSelected() > 0)
    {
      charge& q = simarr[qedit_cb->getSelected() - 1];
      UPDATE_UNFOCUSED(qedit_label,setText(q.label.c_str()));
      std::wstringstream s;
      s << std::setprecision(std::numeric_limits<FLType>::digits10) << q.r;
      UPDATE_UNFOCUSED(qedit_radius,setText(s.str().c_str()));
      s.str(std::wstring());
      s << std::setprecision(std::numeric_limits<FLType>::digits10) << q.m;
      UPDATE_UNFOCUSED(qedit_mass,setText(s.str().c_str()));
      s.str(std::wstring());
      s << std::setprecision(std::numeric_limits<FLType>::digits10) << q.q;
      UPDATE_UNFOCUSED(qedit_charge,setText(s.str().c_str()));
      s.str(std::wstring());
      ((u32 *) qedit_colorbuttonimg->getImage()->lock())[0]=q.col.color;
      qedit_colorbuttonimg->getImage()->unlock();
      qedit_colorbuttonimg->setColor(q.col);
      UPDATE_UNFOCUSED(qedit_deformable,setChecked(q.deformable));
      UPDATE_UNFOCUSED(qedit_partcharge,setPos(q.part_charge));
      s << std::setprecision(std::numeric_limits<float>::digits10) << q.polarizability;
      UPDATE_UNFOCUSED(qedit_polariz,setText(s.str().c_str()));
      s.str(std::wstring());
    }
}

#define CHECKEY(x) keyact[p.keysettings[x]]
//Uma forma abreviada de verificar
//se dada tecla está premida.
#define CHECKBUTTON(x) (event.GUIEvent.EventType == EGET_BUTTON_CLICKED && event.GUIEvent.Caller->getID() == (x))
#define CHECKMENU(x) (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED && \
                      ((IGUIContextMenu *) event.GUIEvent.Caller)       \
                      ->getItemCommandId(((IGUIContextMenu *) event.GUIEvent.Caller) \
                                         ->getSelectedItem()) == (x))
#define CHECKCOMBOBOX(x) (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED && event.GUIEvent.Caller->getID() == (x))
#define CHECKEDITBOX(x) ( (event.GUIEvent.EventType == EGET_EDITBOX_ENTER || \
                           ( event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST && \
                             event.GUIEvent.Caller->getType() == EGUIET_EDIT_BOX )) && \
                          event.GUIEvent.Caller->getID() == (x) )
//Para facilitar a escrita...

bool handle_gui(props &p, const irr::SEvent &event, const std::vector<bool> &keyact)
{
  //Primeiro: menus

  //a) menu "File"

  if(CHECKMENU(MenuFNew))
    {
      size_t i;
      for(i = 0; i < p.simarr.size(); ++i)
        {
          p.simarr[i].clear();
        }
      irr::core::vector3d<float> temp[] = { p.c_lpos, p.c_lvel, p.c_lacc,
                                            p.c_avel, p.c_aacc, p.lookatv,
                                            p.sidev, p.upv };
      generate_default_config(p);
      generate_aux_config(p);
      p.c_lpos = temp[0];
      p.c_lvel = temp[1];
      p.c_lacc = temp[2];
      p.c_avel = temp[3];
      p.c_aacc = temp[4];
      p.lookatv = temp[5];
      p.sidev = temp[6];
      p.upv = temp[7];
      p.changed = false;
      p.update_chargedit();
      p.update_qecombobox();
      p.update_simpareditor();
      p.timetotal = 0;
    }
  else if(CHECKMENU(MenuFOpen) && p.opendialog == nullptr)
    {
      p.create_load_dialog();
      p.blockinteract = true;
    }
  else if(CHECKMENU(MenuFOpenSimple) && p.opendialog == nullptr)
    {
      p.create_simple_load_dialog();
      p.blockinteract = true;
    }
  else if(CHECKMENU(MenuFSave))
    {
      if(p.loaded.size())
        {
          full_save_config(p.loaded.c_str(), p);
          p.changed = false;
        }
      else if(p.opendialog == nullptr)
        {
          p.create_save_dialog();
          p.blockinteract = true;
        }
    }
  else if(CHECKMENU(MenuFSaveAs) && p.opendialog == nullptr)
    {
      p.create_save_dialog();
      p.blockinteract = true;
    }
  else if(CHECKMENU(MenuFExit))
    {
      p.device->closeDevice();
      return true;
    }

  //b) menu "Edit"

  else if(CHECKMENU(MenuESimPar))
    {
      if(p.simpareditor == nullptr)
        {
          p.create_simpareditor();
          p.menu->getSubMenu(MenuEdit - MenuFile)->setItemChecked(MenuESimPar-MenuESimPar,true);
        }
      else
        {
          p.menu->getSubMenu(MenuEdit - MenuFile)->setItemChecked(MenuESimPar-MenuESimPar,false);
          if(p.simpareditbgcolor != nullptr)
            {
              p.simpareditbgcolor->getParent()->remove();
              p.simpareditbgcolor = nullptr;
            }
          if(p.simpareditbndcolor != nullptr)
            {
              p.simpareditbndcolor->getParent()->remove();
              p.simpareditbndcolor = nullptr;
            }
          p.simpareditor->remove();
          p.simpareditor = nullptr;
        }
    }
  else if(CHECKMENU(MenuECharges))
    {
      if(p.chargeditor == nullptr)
        {
          p.create_chargeditor();
          p.chargedit_select(0);
          p.menu->getSubMenu(MenuEdit - MenuFile)->setItemChecked(MenuECharges-MenuESimPar,true);
        }
      else
        {
          p.menu->getSubMenu(MenuEdit - MenuFile)->setItemChecked(MenuECharges-MenuESimPar,false);
          if(p.qedit_color != nullptr)
            {
              p.qedit_color->getParent()->remove();
              p.qedit_color = nullptr;
            }
          p.chargeditor->remove();
          p.chargeditor = nullptr;
        }
    }

  else if(CHECKMENU(MenuEClearHist))
    {
      size_t i;
      p.hist.reset();
      for(i = 0; i < p.simarr.size(); ++i)
      {
        p.simarr[i].acc.set_all(0);
      }
    }

  //c) menu "View"

  else if(CHECKMENU(MenuVCam))
    {
      if(p.cameditor == nullptr)
        {
          p.create_cameditor();
          p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVCam-MenuVCam,true);
        }
      else
        {
          p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVCam-MenuVCam,false);
          p.cameditor->remove();
          p.cameditor = nullptr;
        }
    }
  else if(CHECKMENU(MenuVRec))
    {
      if(p.recordwindow == nullptr)
        {
          p.create_recordwindow();
          p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRec-MenuVCam,true);
        }
      else
        {
          p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRec-MenuVCam,false);
          p.recordwindow->remove();
          p.recordwindow = nullptr;
        }
    }
  else if(CHECKMENU(MenuVRecing))
    {
      if(!p.recb->isPressed())
        {
          if(p.recsave.size() == 0 && !p.out.is_open())
            {
              p.timetotal = 0;
              reset_history_times(p, true);
              p.play = false;
              if(p.opendialog == nullptr)
                {
                  p.create_record_dialog();
                }
            }
          else if(!p.out.is_open())
            {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              p.timetotal = 0;
              reset_history_times(p, true);
              p.play = false;
              p.out.open(converter.to_bytes(p.recsave).c_str());
              if(!p.out.is_open())
                {
                  p.recb->setPressed(false);
                  p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,false);
                  std::cerr << "Error: unable to open file '" << p.recsave.c_str()
                            << "' for data recording.\n" << std::endl;
                }
              else
                {
                  p.out << p.simarr.size() << "\n";
                  for(size_t j = 0; j < p.simarr.size(); ++j)
                    {
                      p.out << converter.to_bytes(p.simarr[j].label) << "\n";
                    }
                  p.out << std::endl;
                  p.recb->setPressed(true);
                  p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,true);
                }
            }
          else
            {
              p.recb->setPressed(true);
              p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,true);
            }
        }
      else
        {
          p.recb->setPressed(false);
          p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,false);
        }
    }
  else if(CHECKMENU(MenuVPlayPause))
    {
      p.play = !p.play;
      //O update resolve o setChecked.
      //Só porque seriam, neste caso,
      //demasiados os sítios onde o play se alterna
      //para se estar a fazer o setChecked lá...
      //E a eficiência não se queixa por aí além.
    }

  //d) menu "Options"

  else if(CHECKMENU(MenuOComp))
    {
      if(p.optionwindow == nullptr)
        {
          p.create_optionwindow();
          p.menu->getSubMenu(MenuOptions - MenuFile)->setItemChecked(MenuOComp-MenuOComp,true);
        }
      else
        {
          p.optionwindow->remove();
          p.menu->getSubMenu(MenuOptions - MenuFile)->setItemChecked(MenuOComp-MenuOComp,false);
          p.optionwindow = nullptr;
        }
    }
  else if(CHECKMENU(MenuOKeys))
    {
      if(p.keywindow == nullptr)
        {
          p.create_keywindow();
          p.menu->getSubMenu(MenuOptions - MenuFile)->setItemChecked(MenuOKeys-MenuOComp,true);
        }
      else
        {
          p.keywindow->remove();
          p.menu->getSubMenu(MenuOptions - MenuFile)->setItemChecked(MenuOKeys-MenuOComp,false);
          p.keywindow = nullptr;
        }
    }
  else if(CHECKMENU(MenuOSave))
    {
      if(p.loadedsettings.size())
        {
          save_settings(p.loadedsettings.c_str(), p);
          p.changedsettings = false;
        }
      else if(p.opendialog == nullptr)
        {
          p.create_optsave_dialog();
          p.blockinteract = true;
        }
    }
  else if(CHECKMENU(MenuOSaveAs) && p.opendialog == nullptr)
    {
      p.create_optsave_dialog();
      p.blockinteract = true;
    }
  else if(CHECKMENU(MenuOLoad) && p.opendialog == nullptr)
    {
      p.create_optload_dialog();
      p.blockinteract = true;
    }
  else if (CHECKMENU(MenuOSetDefault))
    {
      save_settings(DEFAULT_SETTINGS, p);
    }
  else if (CHECKMENU(MenuOReset))
    {
      load_settings(DEFAULT_SETTINGS, p);
      p.changedsettings = true;
      p.update_optionwindow();
      p.update_keywindow();
    }

  //2.718) menu "About"

  else if(CHECKMENU(MenuAHelp))
    {
      if(p.helpwindow == nullptr)
        {
          p.create_help_window();
          p.menu->getSubMenu(MenuAbout - MenuFile)->setItemChecked(MenuAHelp-MenuAHelp,true);
        }
      else
        {
          p.menu->getSubMenu(MenuAbout - MenuFile)->setItemChecked(MenuAHelp-MenuAHelp,false);
          p.helpwindow->remove();
          p.helpwindow = nullptr;
        }
    }
  else if(CHECKMENU(MenuACreds))
    {
      if(p.creditwindow == nullptr)
        {
          p.create_credits_window();
          p.menu->getSubMenu(MenuAbout - MenuFile)->setItemChecked(MenuACreds-MenuAHelp,true);
        }
      else
        {
          p.menu->getSubMenu(MenuAbout - MenuFile)->setItemChecked(MenuACreds-MenuAHelp,false);
          p.creditwindow->remove();
          p.creditwindow = nullptr;
        }
    }

  //Acabaram-se os menus.

  //Color picker do editor de cargas
  else if(event.GUIEvent.EventType == EGET_FILE_SELECTED && event.GUIEvent.Caller->getID() == GUIQEColor)
    {
      p.simarr[p.qedit_cb->getSelected()-1].col = p.qedit_color->getSelectedColor();
      ((u32 *) p.qedit_colorbuttonimg->getImage()->lock())[0]=p.qedit_color->getSelectedColor().color;
      p.qedit_colorbuttonimg->getImage()->unlock();
      p.qedit_colorbuttonimg->setColor(p.qedit_color->getSelectedColor());
      p.smgr->getMeshManipulator()->setVertexColors(p.simarr[p.qedit_cb->getSelected()-1].node->getMesh(), p.qedit_color->getSelectedColor());
      p.simarr[p.qedit_cb->getSelected()-1].node->getMesh()->setDirty();
      p.qedit_color->getParent()->remove();
      p.qedit_color = nullptr;
      p.changed = true;
    }

  //Color picker da cor do background
  else if(event.GUIEvent.EventType == EGET_FILE_SELECTED && event.GUIEvent.Caller->getID() == GUISimparBGCol)
    {
      p.background = p.simpareditbgcolor->getSelectedColor();
      ((u32 *) p.simpareditbgcimg->getImage()->lock())[0]=p.simpareditbgcolor->getSelectedColor().color;
      p.simpareditbgcimg->getImage()->unlock();
      p.simpareditbgcimg->setColor(p.simpareditbgcolor->getSelectedColor());

      p.simpareditbgcolor->getParent()->remove();
      p.simpareditbgcolor = nullptr;
      p.changed = true;
    }

  //Color picker da cor dos elementos da fronteira
  else if(event.GUIEvent.EventType == EGET_FILE_SELECTED && event.GUIEvent.Caller->getID() == GUISimparBndCol)
    {
      size_t j;
      p.boundcolor = p.simpareditbndcolor->getSelectedColor();
      ((u32 *) p.simpareditbndcimg->getImage()->lock())[0]=p.simpareditbndcolor->getSelectedColor().color;
      p.simpareditbndcimg->getImage()->unlock();
      p.simpareditbndcimg->setColor(p.simpareditbndcolor->getSelectedColor());

      for(j = 0; j < 6; ++j)
        {
          p.smgr->getMeshManipulator()->setVertexColors(p.bounds[j]->getMesh(), p.boundcolor);
        }

      p.simpareditbndcolor->getParent()->remove();
      p.simpareditbndcolor = nullptr;
      p.changed = true;
    }

  //Selectores de ficheiros
  else if(event.GUIEvent.EventType == EGET_FILE_SELECTED && p.opendialog != nullptr)
    {
      std::wstring name(p.opendialog->getFileName());
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      irr::core::vector3d<float> temp[] = { p.c_lpos, p.c_lvel, p.c_lacc,
                                            p.c_avel, p.c_aacc, p.lookatv,
                                            p.sidev, p.upv };
      switch(event.GUIEvent.Caller->getID())
        {
        case GUILoadDialog:
          p.chargedit_select(0);
          full_load_config(converter.to_bytes(name).c_str(), p);
          p.update_qecombobox();
          p.update_simpareditor();
          p.blockinteract = false;
          break;
        case GUISimpleLoadDialog:
          p.chargedit_select(0);
          load_config(converter.to_bytes(name).c_str(), p);
          generate_aux_config(p);
          p.c_lpos = temp[0];
          p.c_lvel = temp[1];
          p.c_lacc = temp[2];
          p.c_avel = temp[3];
          p.c_aacc = temp[4];
          p.lookatv = temp[5];
          p.sidev = temp[6];
          p.upv = temp[7];
          p.loaded = converter.to_bytes(name);
          p.play = false;
          p.blockinteract = false;
          p.changed = false;
          p.update_qecombobox();
          p.update_simpareditor();
          break;
        case GUIOptLoadDialog:
          load_settings(converter.to_bytes(name).c_str(),p);
          p.update_optionwindow();
          p.changedsettings = false;
          break;
        case GUISaveDialog:
          full_save_config(converter.to_bytes(name).c_str(), p);
          p.blockinteract = false;
          p.changed = false;
          break;
        case GUIOptSaveDialog:
          save_settings(converter.to_bytes(name).c_str(), p);
          p.changedsettings = false;
          break;
        case GUIDataSaveDialog:
          p.recsave = name;
          p.out.open(converter.to_bytes(name).c_str());
          if(!p.out.is_open())
            {
              p.recb->setPressed(false);
              p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,false);
              std::cerr << "Error: unable to open file '" << name.c_str()
                        << "' for data recording.\n" << std::endl;
            }
          else
            {
              p.out << p.simarr.size() << "\n";
              for(size_t j = 0; j < p.simarr.size(); ++j)
                {
                  p.out << converter.to_bytes(p.simarr[j].label) << "\n";
                }
              p.out << std::endl;
            }
          p.update_recordwindow();
          break;
        default:
          break;
        }
      if(p.opendialog != nullptr)
        //Se tudo correr bem,
        //p.opendialog só é não-nulo
        //se estiver aberto, caso em que
        //nada mais poderá ser fechado
        //para além dele por ser modal.
        {
          p.blockinteract = false;
          p.opendialog->remove();
          p.opendialog = nullptr;
        }
    }
  //Fim dos selectores de ficheiros

  //Menu do rato

  else if(CHECKMENU(MMenuStop))
    {
      p.simarr[p.mmenusel].stop(p.timescale*p.deltat);
      p.changed = true;
    }
  else if(CHECKMENU(MMenuMove))
    {
      p.toolinuse = ToolbarMove - ToolbarMove;
      p.toolbarbuttons[ToolbarMove-ToolbarMove]->setPressed(true);
      p.change_selected(p.mmenusel);
    }
  else if(CHECKMENU(MMenuClone))
    {
      charge tempq(p.simarr[p.mmenusel]);
      tempq.reset_position(p);
      p.add_charge(tempq);
      p.changed = true;
    }
  else if (CHECKMENU(MMenuEdit))
    {
      if(p.chargeditor == nullptr)
        {
          p.create_chargeditor();
          p.chargedit_select(p.mmenusel+1);
          p.menu->getSubMenu(MenuEdit - MenuFile)->setItemChecked(MenuECharges-MenuESimPar,true);
        }
      else
        {
          p.chargedit_select(p.mmenusel+1);
        }
    }
  else if(CHECKMENU(MMenuDelete))
    {
      p.delete_charge(p.mmenusel);
      p.changed = true;
    }

  //Fim do menu do rato

  //Janela de edição de cargas

  else if(CHECKBUTTON(GUIQEColorButton))
    {
      if(p.qedit_color == nullptr)
        {
          p.create_qecolorpicker();
        }
      else
        {
          p.qedit_color->getParent()->setVisible(!p.qedit_color->getParent()->isVisible());
          p.guienv->getRootGUIElement()->bringToFront(p.qedit_color->getParent());
        }
    }

  else if(CHECKCOMBOBOX(GUIQEChoose))
    {
      p.blockinteract = false;
      p.chargedit_select(p.qedit_cb->getSelected());
    }

  else if(CHECKBUTTON(GUIQEDelete))
    {
      p.delete_charge(p.qedit_cb->getSelected()-1);
      p.changed = true;
    }

  else if(CHECKBUTTON(GUIQEClone))
    {
      charge tempq(p.simarr[p.qedit_cb->getSelected()-1]);
      tempq.reset_position(p);
      p.add_charge(tempq);
      p.chargedit_select(p.simarr.size());
      p.changed = true;
    }

  else if(CHECKBUTTON(GUIQEMove))
    {
      p.toolinuse = ToolbarMove - ToolbarMove;
      p.toolbarbuttons[ToolbarMove-ToolbarMove]->setPressed(true);
      p.change_selected(p.qedit_cb->getSelected()-1);
    }

  //Fim da janela de edição de cargas

  //Janela de edição dos parâmetros da simulação.

  else if(CHECKBUTTON(GUISimparAutosizeHist))
    {
      size_t new_size = autosize_hist(p);
      p.hist.resize_records(new_size);
      std::wstringstream s;
      s << new_size;
      p.simparedithistsize->setText(s.str().c_str());
      p.changed = true;
    }

  else if(CHECKBUTTON(GUISimparBGColButton))
    {
      if(p.simpareditbgcolor == nullptr)
        {
          p.create_backgroundcolorpicker();
        }
      else
        {
          p.simpareditbgcolor->getParent()->setVisible(!p.simpareditbgcolor->getParent()->isVisible());
          p.guienv->getRootGUIElement()->bringToFront(p.simpareditbgcolor->getParent());
        }
    }

  else if(CHECKBUTTON(GUISimparBndColButton))
    {
      if(p.simpareditbndcolor == nullptr)
        {
          p.create_boundcolorpicker();
        }
      else
        {
          p.simpareditbndcolor->getParent()->setVisible(!p.simpareditbndcolor->getParent()->isVisible());
          p.guienv->getRootGUIElement()->bringToFront(p.simpareditbndcolor->getParent());
        }
    }

  //Fim do simparedit.

  //Janela dos keybindings
  else if(event.GUIEvent.EventType == EGET_BUTTON_CLICKED &&
          event.GUIEvent.Caller->getID() >= GUIKey0 &&
          event.GUIEvent.Caller->getID() < GUIKey0+TOTALKEYS)
    {
      size_t i;
      for (i = 0; i < TOTALKEYS; ++i)
        {
          if(i == event.GUIEvent.Caller->getID()-GUIKey0)
            {
              p.keywindowkeys[i]->setText(L"PRESS NEW KEY");
              p.expecting_keybinding = i;
            }
          else
            {
              p.keywindowkeys[i]->setPressed(false);
            }
        }
      p.blockinteract = true;
    }
  //Fim da janela dos keybindings

  //Janela de gravação.

  else if (CHECKBUTTON(GUIRWFileSelect))
    {
      if(p.opendialog == nullptr)
        {
          p.create_record_dialog();
        }
    }

  //Fim da janela de gravação.

  //Botões do menu cá de baixo
  else if (CHECKBUTTON(GUIPlayPause))
    {
      p.play = !p.play;
    }
  else if (CHECKBUTTON(GUIRecord) && p.recb->isPressed())
    {
      if(p.recsave.size() == 0 && !p.out.is_open())
        {
          p.timetotal = 0;
          reset_history_times(p, true);
          p.play = false;
          if(p.opendialog == nullptr)
            {
              p.create_record_dialog();
            }
        }
      else if(!p.out.is_open())
        {
          std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
          p.timetotal = 0;
          reset_history_times(p, true);
          p.play = false;
          p.out.open(converter.to_bytes(p.recsave).c_str());
          if(!p.out.is_open())
            {
              p.recb->setPressed(false);
              p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,false);
              std::cerr << "Error: unable to open file '" << p.recsave.c_str()
                        << "' for data recording.\n" << std::endl;
            }
          else
            {
              p.out << p.simarr.size() << "\n";
              for(size_t j = 0; j < p.simarr.size(); ++j)
                {
                  p.out << converter.to_bytes(p.simarr[j].label) << "\n";
                }
              p.out << std::endl;
              p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,true);
            }
        }
      else
        {
          p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,true);
        }
    }
  else if (CHECKBUTTON(GUIRecord))
    {
      p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,false);
    }

  else if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED &&
           event.GUIEvent.Caller->getID() >= ToolbarMove && event.GUIEvent.Caller->getID() < ToolbarLast )
    {
      size_t i;
      p.change_selected(-1);
      p.toolinuse = (((IGUIButton*) event.GUIEvent.Caller)->isPressed() ? event.GUIEvent.Caller->getID() - ToolbarMove : -1);
      for (i = 0; i < ToolbarLast-ToolbarMove ; ++i)
        {
          if (i == p.toolinuse)
            {
              continue;
            }
          p.toolbarbuttons[i]->setPressed(false);
        }
    }

  //Edit Boxes
  else if ( event.GUIEvent.EventType == EGET_EDITBOX_ENTER ||
            (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST &&
             event.GUIEvent.Caller->getType() == EGUIET_EDIT_BOX    ) )
    {
      wchar_t *t;
      long int tempint;
      size_t tempsizet;
      vector3df tempv3f;
      SII::SI_value<FLType, std::wstring> val;
      fspoint<FLType> tempfsp;
      std::wstring txt(event.GUIEvent.Caller->getText());
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      std::wstringstream stream;
      //Dado que os autores não puderam testar o programa em ambiente Linux,
      //é possível que surjam problemas em termos do display da label,
      //dadas as diferenças na codificação entre os dois sistemas operativos.
      //Caso tal se verifique, o uso de std::covecvt_utf8<wchar_t>
      //poderá ser necessário para os resolver;
      //não nos é, no entanto, possível saber se funcionará a priori.
      //Lamentamos por esta eventualidade...
      switch(event.GUIEvent.Caller->getID())
        {
        case GUIQELabel:
          p.simarr[p.qedit_cb->getSelected()-1].label = p.qedit_label->getText();
          if(p.mousemenu != nullptr)
            {
              p.mousemenu->setItemText(0,p.qedit_label->getText());
            }
          p.changed = true;
          p.update_qecombobox();
          break;
        case GUIQEPosX:
        case GUIQEPosY:
        case GUIQEPosZ:
          tempint = event.GUIEvent.Caller->getID() - GUIQEPosX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.simarr[p.qedit_cb->getSelected()-1].pos[tempint] = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUIQEVelX:
        case GUIQEVelY:
        case GUIQEVelZ:
          tempint = event.GUIEvent.Caller->getID() - GUIQEVelX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(1,-1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.simarr[p.qedit_cb->getSelected()-1].vel[tempint] = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUIQEMomX:
        case GUIQEMomY:
        case GUIQEMomZ:
          tempint = event.GUIEvent.Caller->getID() - GUIQEMomX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(1,-1,1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            tempfsp = p.simarr[p.qedit_cb->getSelected()-1].v_to_p(p.lightspeed);
            tempfsp[tempint] = val.value;
            p.simarr[p.qedit_cb->getSelected()-1].vel = p.simarr[p.qedit_cb->getSelected()-1].p_to_v(tempfsp, p.lightspeed);
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUIQERadius:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if((!val.dimensions.check_dimensions(1) && !val.dimensions.check_dimensions()) || val.value < 0)
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.simarr[p.qedit_cb->getSelected()-1].r = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUIQEMass:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(0,0,1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.simarr[p.qedit_cb->getSelected()-1].m = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUIQECharge:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(0,1,0,0,1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.simarr[p.qedit_cb->getSelected()-1].q = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUIQEPolarizability:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          p.simarr[p.qedit_cb->getSelected()-1].polarizability = val.value;
          stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
          event.GUIEvent.Caller->setText(stream.str().c_str());
          p.changed = true;
          break;
        case GUICamPosX:
        case GUICamPosY:
        case GUICamPosZ:
          tempint = event.GUIEvent.Caller->getID() - GUICamPosX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            getv3df(p.c_lpos,tempint) = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUICamVelX:
        case GUICamVelY:
        case GUICamVelZ:
          tempint = event.GUIEvent.Caller->getID() - GUICamVelX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(1,-1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            getv3df(p.c_lvel,tempint) = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUICamAccX:
        case GUICamAccY:
        case GUICamAccZ:
          tempint = event.GUIEvent.Caller->getID() - GUICamAccX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(1,-2) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            getv3df(p.c_lacc,tempint) = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUICamAPosX:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.rot.toEuler(tempv3f);
            tempv3f.X = val;
            p.rot.set(tempv3f);
            p.update_cam_vectors();
            p.changed = true;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value*180/M_PI << L"º";
            event.GUIEvent.Caller->setText(stream.str().c_str());
          }
          break;
        case GUICamAPosY:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.rot.toEuler(tempv3f);
            tempv3f.Y = val;
            p.rot.set(tempv3f);
            p.update_cam_vectors();
            p.changed = true;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value*180/M_PI << L"º";
            event.GUIEvent.Caller->setText(stream.str().c_str());
          }
          break;
        case GUICamAPosZ:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.rot.toEuler(tempv3f);
            tempv3f.Z = val;
            p.rot.set(tempv3f);
            p.update_cam_vectors();
            p.changed = true;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value*180/M_PI << L"º";
            event.GUIEvent.Caller->setText(stream.str().c_str());
          }
          break;
        case GUICamAVelX:
        case GUICamAVelY:
        case GUICamAVelZ:
          tempint = event.GUIEvent.Caller->getID() - GUICamAVelX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(0,-1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            getv3df(p.c_avel,tempint) = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value*180/M_PI << L"º";
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUICamAAccX:
        case GUICamAAccY:
        case GUICamAAccZ:
          tempint = event.GUIEvent.Caller->getID() - GUICamAAccX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(0,-1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            getv3df(p.c_aacc,tempint) = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value*180/M_PI << L"º";
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          break;
        case GUISimparLowLimitsX:
        case GUISimparLowLimitsY:
        case GUISimparLowLimitsZ:
        case GUISimparUpLimitsX:
        case GUISimparUpLimitsY:
        case GUISimparUpLimitsZ:
          tempint = event.GUIEvent.Caller->getID() - GUISimparLowLimitsX;
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(!val.dimensions.check_dimensions(1) && !val.dimensions.check_dimensions())
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.limits[tempint] = val.value;
            if(tempint > 2)
              {
                p.limits[tempint % 3] = std::min(p.limits[tempint % 3], val.value);
              }
            else
              {
                p.limits[tempint % 3 + 3] = std::max(p.limits[tempint % 3 + 3], val.value);
              }
            p.update_simpareditor();
            p.changed = true;
          }
          break;
        case GUISimparEExtX:
        case GUISimparEExtY:
        case GUISimparEExtZ:
          tempint = event.GUIEvent.Caller->getID() - GUISimparEExtX;
          p.Eext[tempint] = converter.to_bytes(txt);
          p.Efunc[tempint].Parse(p.Eext[tempint],"x,y,z,t");
          p.Efunc[tempint].Optimize();
          p.changed = true;
          break;
        case GUISimparBExtX:
        case GUISimparBExtY:
        case GUISimparBExtZ:
          tempint = event.GUIEvent.Caller->getID() - GUISimparBExtX;
          p.Bext[tempint] = converter.to_bytes(txt);
          p.Bfunc[tempint].Parse(p.Bext[tempint],"x,y,z,t");
          p.Bfunc[tempint].Optimize();
          p.changed = true;
          break;
        case GUISimparLightspeed:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if((!val.dimensions.check_dimensions(1,-1) && !val.dimensions.check_dimensions()) || val.value < 0)
          {
            event.GUIEvent.Caller->setText(L"ERROR");
          }
          else
          {
            p.lightspeed = val.value;
            stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
            event.GUIEvent.Caller->setText(stream.str().c_str());
            p.changed = true;
          }
          p.changed = true;
          break;
        case GUISimparHistSize:
          tempsizet = wcstoul(event.GUIEvent.Caller->getText(), &t, 10);
          if(tempsizet < LINEAR_SEARCH_THRESHOLD || tempsizet > MAXIMUM_HISTORY)
            {
              p.simparedithistsize->setText(std::to_wstring(p.hist.get_numr(true)).c_str());
            }
          else
            {
              p.hist.resize_records(tempsizet);
              p.changed = true;
            }
          break;
        case GUIRecordNum:
          tempint = wcstol(((IGUIEditBox*) event.GUIEvent.Caller)->getText(), &t, 10);
          if(*t != '\0' || tempint < 0)
            {
              ((IGUIEditBox*) event.GUIEvent.Caller)->setText(L"ERROR");
            }
          else
            {
              p.totalprint = (size_t) tempint;
            }
          if(p.recordwindow != nullptr && event.GUIEvent.Caller->getParent() != p.recordwindow)
            {
              p.recw_recordnum->setText(p.recordnum->getText());
            }
          else if(event.GUIEvent.Caller->getParent() == p.recordwindow)
            {
              p.recordnum->setText(p.recw_recordnum->getText());
            }
          break;
        case GUIRecordInterval:
          tempint = wcstol(((IGUIEditBox*) event.GUIEvent.Caller)->getText(), &t, 10);
          if(*t != '\0' || tempint < 0)
            {
              ((IGUIEditBox*) event.GUIEvent.Caller)->setText(L"ERROR");
            }
          else
            {
              p.printerval = (size_t) tempint;
            }
          if(p.recordwindow != nullptr && event.GUIEvent.Caller->getParent() != p.recordwindow)
            {
              p.recw_recordint->setText(p.recordint->getText());
            }
          else if(event.GUIEvent.Caller->getParent() == p.recordwindow)
            {
              p.recordint->setText(p.recw_recordint->getText());
            }
          break;
        case GUIEditSpace:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if((!val.dimensions.check_dimensions(1) && !val.dimensions.check_dimensions()) || val.value < 0)
          {
            p.spacedit->setText(L"ERROR");
            if(p.cameditor != nullptr)
              {
                p.cameditspacedit->setText(L"ERROR");
              }
          }
          else
          {
              p.scale = val.value;
              p.changed = true;
          }
          break;
        case GUIEditTime:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if((!val.dimensions.check_dimensions(1) && !val.dimensions.check_dimensions()) || val.value < 0)
          {
            p.timedit->setText(L"ERROR");
            if(p.simpareditor != nullptr)
              {
                p.simparedittimedit->setText(L"ERROR");
              }
          }
          else
          {
              p.timescale = val.value;
              p.changed = true;
          }
          break;
        case GUIRWFilePath:
          if(!p.recb->isPressed())
            {
              p.recsave = txt;
            }
          break;
        case GUIOptITCEdit:
          tempint = std::wcstoll(txt.c_str(),&t,10);
          if(*t != '\0' || tempint < 0)
            {
              p.opt_ITCedit->setText(L"ERROR");
            }
          else
            {
              p.itercount = (size_t) tempint;
            }
          p.changedsettings = true;
          break;
        case GUIOptROCEdit:
          val = p.interpreter.interpret(event.GUIEvent.Caller->getText());
          if(val.value < 0)
          {
              p.opt_ROCedit->setText(L"ERROR");
          }
          else
          {
              p.rate_of_change = val.value;
              stream << std::setprecision(std::numeric_limits<FLType>::digits10) << val.value;
              event.GUIEvent.Caller->setText(stream.str().c_str());
              p.changedsettings = true;
          }
          break;
        case GUIOptThreadEdit:
          tempint = std::wcstoll(txt.c_str(),&t,10);
          if(*t != '\0' || tempint < 0)
            {
              p.opt_threadedit->setText(L"ERROR");
            }
          else
            {
              p.numthreads = std::min((size_t) tempint, (size_t) omp_get_num_procs());
              omp_set_num_threads(p.numthreads);
              p.opt_threadedit->setText(std::to_wstring(p.numthreads).c_str());
            }
          p.changedsettings = true;
          break;
        default:
          break;
        }
      if(event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
        {
          p.blockinteract = false;
        }
    }

  //Scroll bars

  else if(event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED)
    {
      switch(event.GUIEvent.Caller->getID())
        {
        case GUIOptROCScroll:
          if(p.opt_ROCscroll->getPos() < 0 || p.opt_ROCscroll->getPos() > 10000 )
            {
              p.rate_of_change = p.past_roc*pow(2,((float)p.opt_ROCscroll->getPos()-5000.0f)/5000.0f);
              p.past_roc = p.rate_of_change;
              p.opt_ROCscroll->setPos(5000);
              p.device->getCursorControl()->setPosition((p.opt_ROCscroll->getAbsolutePosition().LowerRightCorner
                                                         + p.opt_ROCscroll->getAbsolutePosition().UpperLeftCorner)/2);
            }
          else
            {
              p.rate_of_change = p.past_roc*pow(2,((float)p.opt_ROCscroll->getPos()-5000.0f)/5000.0f);
            }
          p.changedsettings = true;
          break;
        case GUIOptITCScroll:
          p.itercount = p.opt_ITCscroll->getPos();
          p.opt_ITCedit->setText(std::to_wstring(p.itercount).c_str());
          p.changedsettings = true;
          break;
        case GUIOptThreadScroll:
          p.numthreads = p.opt_threadscroll->getPos();
          omp_set_num_threads(p.numthreads);
          p.opt_threadedit->setText(std::to_wstring(p.numthreads).c_str());
          p.changedsettings = true;
          break;
        case GUIScrollSpace:
          if(p.spacescroll->getPos() < 0 || p.spacescroll->getPos() > 10000 )
            {
              p.scale = p.past_scale*pow(2,((FLType)p.spacescroll->getPos()-5000.0)/5000.0);
              p.past_scale = p.scale;
              p.spacescroll->setPos(5000);
              if(p.cameditor != nullptr)
                {
                  p.cameditspacescroll->setPos(5000);
                }
              p.device->getCursorControl()->setPosition((p.spacescroll->getAbsolutePosition().LowerRightCorner
                                                         + p.spacescroll->getAbsolutePosition().UpperLeftCorner)/2);
            }
          else
            {
              p.scale = p.past_scale*pow(2,((FLType)p.spacescroll->getPos()-5000.0)/5000.0);
              if(p.cameditor != nullptr)
                {
                  p.cameditspacescroll->setPos(p.spacescroll->getPos());
                }
            }
          p.changed = true;
          break;
        case GUICamScrollSpace:
          if(p.cameditspacescroll->getPos() < 0 || p.cameditspacescroll->getPos() > 10000 )
            {
              p.scale = p.past_scale*pow(2,((FLType)p.cameditspacescroll->getPos()-5000.0)/5000.0);
              p.past_scale = p.scale;
              p.cameditspacescroll->setPos(5000);
              p.spacescroll->setPos(5000);
              p.device->getCursorControl()->setPosition((p.cameditspacescroll->getAbsolutePosition().LowerRightCorner
                                                         + p.cameditspacescroll->getAbsolutePosition().UpperLeftCorner)/2);
            }
          else
            {
              p.scale = p.past_scale*pow(2,((FLType)p.cameditspacescroll->getPos()-5000.0)/5000.0);
              p.spacescroll->setPos(p.cameditspacescroll->getPos());
            }
          p.changed = true;
          break;
        case GUIScrollTime:
          if(p.timescroll->getPos() < 0 || p.timescroll->getPos() > 10000 )
            {
              p.timescale = p.past_timescale*pow(2,((FLType)p.timescroll->getPos()-5000.0)/5000.0);
              p.past_timescale = p.timescale;
              p.timescroll->setPos(5000);
              if(p.simpareditor != nullptr)
                {
                  p.simparedittimescroll->setPos(5000);
                }
              p.device->getCursorControl()->setPosition((p.timescroll->getAbsolutePosition().LowerRightCorner
                                                         + p.timescroll->getAbsolutePosition().UpperLeftCorner)/2);
            }
          else
            {
              p.timescale = p.past_timescale*pow(2,((FLType)p.timescroll->getPos()-5000.0)/5000.0);
              if(p.simpareditor != nullptr)
                {
                  p.simparedittimescroll->setPos(p.timescroll->getPos());
                }
            }
          p.changed = true;
          break;
        case GUISimparScrollTime:
          if(p.simparedittimescroll->getPos() < 0 || p.simparedittimescroll->getPos() > 10000 )
            {
              p.timescale = p.past_timescale*pow(2,((FLType)p.simparedittimescroll->getPos()-5000.0)/5000.0);
              p.past_timescale = p.timescale;
              p.simparedittimescroll->setPos(5000);
              p.timescroll->setPos(5000);
              p.device->getCursorControl()->setPosition((p.simparedittimescroll->getAbsolutePosition().LowerRightCorner
                                                         + p.simparedittimescroll->getAbsolutePosition().UpperLeftCorner)/2);
            }
          else
            {
              p.timescale = p.past_timescale*pow(2,((FLType)p.simparedittimescroll->getPos()-5000.0)/5000.0);
              p.timescroll->setPos(p.simparedittimescroll->getPos());
            }
          p.changed = true;
          break;
        case GUIQEPartCharge:
          {
            p.simarr[p.qedit_cb->getSelected()-1].part_charge = p.qedit_partcharge->getPos();
            p.changed = true;
          }
        default:
          break;
        }
    }

  //Checkboxes

  else if(event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED)
    {
      switch(event.GUIEvent.Caller->getID())
        {
        case GUIOptInvx:
          p.invertx = p.opt_invx->isChecked();
          p.changedsettings = true;
          break;
        case GUIOptInvy:
          p.inverty = p.opt_invy->isChecked();
          p.changedsettings = true;
          break;
        case GUIOptRelMov:
          p.relative = p.opt_relmov->isChecked();
          p.changedsettings = true;
          break;
        case GUIQEDeformable:
          {
            p.simarr[p.qedit_cb->getSelected()-1].deformable = p.qedit_deformable->isChecked();
            p.changed = true;
          }
          break;
        default:
          break;
        }
    }

  //Fechamentos de coisas

  else if( event.GUIEvent.EventType == EGET_ELEMENT_CLOSED ||
           event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED )
    {
      if(p.opendialog != nullptr)
        //Se tudo correr bem,
        //p.opendialog só é não-nulo
        //se estiver aberto, caso em que
        //nada mais poderá ser fechado
        //para além dele por ser modal.
        {
          p.blockinteract = false;
          p.opendialog->remove();
          p.opendialog = nullptr;
        }
      p.blockinteract = false;
      if(event.GUIEvent.Caller->getID() == GUIDataSaveDialog)
        {
          p.recb->setPressed(false);
          p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,false);
        }
      else if(event.GUIEvent.Caller->getID() == GUIQEColor)
        {
          p.qedit_color = nullptr;
        }
      else if(event.GUIEvent.Caller->getID() == GUISimparBGCol)
        {
          p.simpareditbgcolor = nullptr;
        }
      else if(event.GUIEvent.Caller->getID() == GUISimparBndCol)
        {
          p.simpareditbndcolor = nullptr;
        }
      else if(event.GUIEvent.Caller->getID() == GUIOptions)
        {
          p.optionwindow = nullptr;
          p.menu->getSubMenu(MenuOptions - MenuFile)->setItemChecked(MenuOComp-MenuOComp,false);

        }
      else if(event.GUIEvent.Caller->getID() == GUIHelp)
        {
          p.helpwindow = nullptr;
          p.menu->getSubMenu(MenuAbout - MenuFile)->setItemChecked(MenuAHelp-MenuAHelp,false);
        }
      else if(event.GUIEvent.Caller->getID() == GUICredits)
        {
          p.creditwindow = nullptr;
          p.menu->getSubMenu(MenuAbout - MenuFile)->setItemChecked(MenuACreds-MenuAHelp,false);
        }
      else if(event.GUIEvent.Caller->getID() == GUISimparEditor)
        {
          p.simpareditor = nullptr;
          p.menu->getSubMenu(MenuEdit - MenuFile)->setItemChecked(MenuESimPar-MenuESimPar,false);
          if(p.simpareditbgcolor != nullptr)
            {
              p.simpareditbgcolor->getParent()->remove();
              p.simpareditbgcolor = nullptr;
            }
          if(p.simpareditbndcolor != nullptr)
            {
              p.simpareditbndcolor->getParent()->remove();
              p.simpareditbndcolor = nullptr;
            }
        }
      else if(event.GUIEvent.Caller->getID() == GUICamEditor)
        {
          p.cameditor = nullptr;
          p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVCam-MenuVCam,false);
        }
      else if(event.GUIEvent.Caller->getID() == GUIQEditor)
        {
          p.chargeditor = nullptr;
          if(p.qedit_color != nullptr)
            {
              p.qedit_color->getParent()->remove();
              p.qedit_color = nullptr;
            }
          p.menu->getSubMenu(MenuEdit - MenuFile)->setItemChecked(MenuECharges-MenuESimPar,false);
        }
      else if(event.GUIEvent.Caller->getID() == GUIMouseMenu)
        {
          p.mousemenu = nullptr;
        }
      else if(event.GUIEvent.Caller->getID() == GUIKeybinding)
        {
          p.keywindow = nullptr;
          p.menu->getSubMenu(MenuOptions - MenuFile)->setItemChecked(MenuOKeys-MenuOComp,false);
        }
    }

  //Estes têm de vir no fim
  else if(event.GUIEvent.EventType == EGET_ELEMENT_FOCUSED)
    {
      if( event.GUIEvent.Caller->getID() != GUITab &&
          event.GUIEvent.Caller->getID() != GUIText &&
          event.GUIEvent.Caller->getType() != EGUIET_BUTTON &&
          event.GUIEvent.Caller->getID() != -1          )
        {
          p.blockinteract = true;
        }
    }
  else if(event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
    {
      if(p.opendialog == nullptr)
        {
          p.blockinteract = false;
        }
    }
  else if((event.GUIEvent.EventType == EGET_ELEMENT_HOVERED
           || event.GUIEvent.EventType == EGET_ELEMENT_FOCUSED ) &&
          event.GUIEvent.Caller->getType() == EGUIET_COMBO_BOX)
    {
      p.blockinteract = true;
    }
  else if((event.GUIEvent.EventType == EGET_ELEMENT_LEFT
           || event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST ) &&
          event.GUIEvent.Caller->getType() == EGUIET_COMBO_BOX)
    {
      p.blockinteract = false;
    }

  return false;
}


class EventReceiver : public IEventReceiver
//Permite receber input do utilizador,
//nomeadamente teclas e rato...
{
public:
  props &p;
  irr::SEvent::SMouseInput mouse;
  irr::SEvent::SKeyInput key;
  std::vector<bool> keyact;
  EventReceiver (props &prop): p(prop)
  {
    keyact = std::vector<bool>(KEY_KEY_CODES_COUNT, false);
  }

  virtual bool OnEvent(const SEvent& event)
  {
    if (event.EventType == EET_KEY_INPUT_EVENT)
      {
        if(p.keywindow != nullptr && p.expecting_keybinding != -1)
          {
            p.keysettings[p.expecting_keybinding] = event.KeyInput.Key;
            p.keywindowkeys[p.expecting_keybinding]->setPressed(false);
            p.keywindowkeys[p.expecting_keybinding]->setText(key_to_name(event.KeyInput.Key).c_str());
            p.expecting_keybinding = -1;
            p.blockinteract = false;
          }
        key = event.KeyInput;
        keyact[event.KeyInput.Key] = event.KeyInput.PressedDown;
      }
    else if (event.EventType == EET_MOUSE_INPUT_EVENT)
      {
        mouse = event.MouseInput;
      }
    else if (event.EventType == EET_GUI_EVENT)
      //Pelo facto de, após os eventos,
      //alguns dos gui elements desaparecerem,
      //a gestão tem de ser feita aqui...
      {
        if(handle_gui(p, event, keyact))
          {
            return true;
          }
      }
    return false;
  }
};


#define HTABSIZE 0
//O tamanho da caixa que conterá os elementos do GUI,
//do lado direito do ecrã.
//(Tamanho HORIZONTAL da caixa VERTICAL,
//slightly misleading se se pensar o contrário.)
#define VTABSIZE 0
//O tamanho da caixa que conterá outros elementos do GUI,
//na parte de baixo do ecrã.
//(Tamanho VERTICAL da caixa HORIZONTAL,
//slightly misleading se se pensar o contrário.)

#define LOWERMENUWIDTH 700
#define LOWERMENUHEIGHT 160

void make_gui(props &p)
//Cria os vários elementos do GUI
//(sem, no entanto, estabelecer
//as suas posições, por isso depender
//das dimensões do ecrã, tendo, por isso,
//de ser actualizado frame a frame).
{
  int i;
  p.arm[0] = p.smgr->addArrowMesh("red",0xFFFF0000,0xFFFF0000);
  p.arm[1] = p.smgr->addArrowMesh("green",0xFF00FF00,0xFF00FF00);
  p.arm[2] = p.smgr->addArrowMesh("blue",0xFF0000FF,0xFF0000FF);
  p.arm[3] = p.smgr->addArrowMesh("greenblue",0xFF00FFFF,0xFF00FFFF);
  p.arm[4] = p.smgr->addArrowMesh("redblue",0xFFFF00FF,0xFFFF00FF);
  p.arm[5] = p.smgr->addArrowMesh("redgreen",0xFFFFFF00,0xFFFFFF00);
  p.arrow[0] = p.smgr->addMeshSceneNode(p.arm[0], nullptr, GUIArrow1,
                                        vector3df(0,0,0), vector3df(0,0,-90));
  p.arrow[1] = p.smgr->addMeshSceneNode(p.arm[1], nullptr, GUIArrow2,
                                        vector3df(0,0,0), vector3df(0,0,0));
  p.arrow[2] = p.smgr->addMeshSceneNode(p.arm[2], nullptr, GUIArrow3,
                                        vector3df(0,0,0), vector3df(90,0,0));
  p.arrow[3] = p.smgr->addMeshSceneNode(p.arm[3], nullptr, GUIArrow4,
                                        vector3df(0,0,0), vector3df(0,0,90));
  p.arrow[4] = p.smgr->addMeshSceneNode(p.arm[4], nullptr, GUIArrow5,
                                        vector3df(0,0,0), vector3df(0,0,180));
  p.arrow[5] = p.smgr->addMeshSceneNode(p.arm[5], nullptr, GUIArrow6,
                                        vector3df(0,0,0), vector3df(-90,0,0));
  p.arrow[0]->setVisible(false);
  p.arrow[1]->setVisible(false);
  p.arrow[2]->setVisible(false);
  p.arrow[3]->setVisible(false);
  p.arrow[4]->setVisible(false);
  p.arrow[5]->setVisible(false);
  p.arrow[0]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  p.arrow[1]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  p.arrow[2]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  p.arrow[3]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  p.arrow[4]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  p.arrow[5]->setMaterialFlag(irr::video::EMF_LIGHTING, false);

  irr::gui::IGUISkin* skin = p.guienv->createSkin(irr::gui::EGST_WINDOWS_CLASSIC);
  for (i = 0; i < EGDC_COUNT; ++i)
    {
      irr::video::SColor col = skin->getColor((irr::gui::EGUI_DEFAULT_COLOR) i);
      col.setAlpha(255);
      skin->setColor((irr::gui::EGUI_DEFAULT_COLOR) i, col);
    }
  irr::gui::IGUIFont* font = p.guienv->getFont("Fonts/text10.xml");
  if(font)
    {
      skin->setFont(font, EGDF_DEFAULT);
    }
  font = p.guienv->getFont("Fonts/text11.xml");
  if(font)
    {
      skin->setFont(font, EGDF_TOOLTIP);
    }
  font = p.guienv->getFont("Fonts/text12.xml");
  if(font)
    {
      skin->setFont(font, EGDF_WINDOW);
    }
  font = p.guienv->getFont("Fonts/SCE12.xml");
  if(font)
    {
      skin->setFont(font, EGDF_BUTTON);
    }
  p.guienv->setSkin(skin);
  skin->drop();
  irr::gui::IGUIContextMenu* submenu;
  p.menu = p.guienv->addMenu();
  p.menu->addItem(L"File",MenuFile, true, true);
  p.menu->addItem(L"Edit",MenuEdit, true, true);
  p.menu->addItem(L"View",MenuView, true, true);
  p.menu->addItem(L"Settings",MenuOptions, true, true);
  p.menu->addItem(L"About",MenuAbout, true, true);
  submenu = p.menu->getSubMenu(0);
  submenu->addItem(L"New",MenuFNew, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Open...",MenuFOpen, true, false);
  submenu->addItem(L"Open without aux...",MenuFOpenSimple, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Save",MenuFSave, true, false);
  submenu->addItem(L"Save as...",MenuFSaveAs, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Quit",MenuFExit, true, false);
  submenu = p.menu->getSubMenu(1);
  submenu->addItem(L"Simulation Parameters...",MenuESimPar, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Charges...",MenuECharges, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Clear History",MenuEClearHist, true, false);
  submenu = p.menu->getSubMenu(2);
  submenu->addItem(L"Change Camera...",MenuVCam, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Recording Properties...",MenuVRec, true, false);
  submenu->addItem(L"Record",MenuVRecing, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Simulate",MenuVPlayPause, true, false);
  submenu = p.menu->getSubMenu(3);
  submenu->addItem(L"Options...",MenuOComp, true, false);
  submenu->addItem(L"Keybindings...",MenuOKeys, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Save Settings",MenuOSave, true, false);
  submenu->addItem(L"Save Settings As...",MenuOSave, true, false);
  submenu->addItem(L"Load Settings...",MenuOLoad, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Save Settings As Default",MenuOSetDefault, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Load Default Settings",MenuOReset, true, false);
  submenu = p.menu->getSubMenu(4);
  submenu->addItem(L"Help...",MenuAHelp, true, false);
  submenu->addSeparator();
  submenu->addItem(L"Credits...",MenuACreds, true, false);

  p.toolbar = p.guienv->addToolBar();
  irr::video::ITexture *image = p.driver->getTexture(L"Icons/move.png"),
    *fallback = p.guienv->getVideoDriver()->addTexture({1, 1}, "fallb", video::ECF_A8R8G8B8);
  ((u32 *) fallback->lock())[0]=0xFF00FF00;\
  fallback->unlock();

#define ADDTOOLBUTTON(INDEX, TEXT)                                      \
  {                                                                     \
    if(!image)                                                          \
      {                                                                 \
        p.toolbarbuttons[(INDEX)-ToolbarMove] = p.toolbar->addButton(INDEX, 0, TEXT, fallback, 0, true, true); \
      }                                                                 \
    else                                                                \
      {                                                                 \
        p.toolbarbuttons[(INDEX)-ToolbarMove] = p.toolbar->addButton(INDEX, 0, TEXT, image, 0, true, true); \
        image->drop();                                                  \
      }                                                                 \
  }

  ADDTOOLBUTTON(ToolbarMove, L"Move Charges");
  image = p.driver->getTexture(L"Icons/stop.png");
  ADDTOOLBUTTON(ToolbarStop, L"Stop Charges");
  image = p.driver->getTexture(L"Icons/edit.png");
  ADDTOOLBUTTON(ToolbarEdit, L"Edit Charges");
  image = p.driver->getTexture(L"Icons/add.png");
  ADDTOOLBUTTON(ToolbarAdd, L"Add Charges");
  image = p.driver->getTexture(L"Icons/clone.png");
  ADDTOOLBUTTON(ToolbarClone, L"Clone Charges");
  image = p.driver->getTexture(L"Icons/delete.png");
  ADDTOOLBUTTON(ToolbarDelete, L"Delete Charges");

#undef ADDTOOLBUTTON

  p.lowermenu = p.guienv->addTab(core::rect<s32>(0,0,10,10),0,GUITab);
  //A posição é actualizada em função do tamanho da janela
  p.lowermenu -> setBackgroundColor(irr::video::SColor(255,255,255,255));
  p.lowermenu -> setDrawBackground(true);
  p.plpausb = p.guienv->addButton( core::rect<s32>((LOWERMENUWIDTH)/2 - LOWERMENUHEIGHT+70, 40, (LOWERMENUWIDTH)/2-10, LOWERMENUHEIGHT-40), p.lowermenu, GUIPlayPause,
                                   L"", L"Play or pause the simulation.");
  irr::video::ITexture* im = p.driver->getTexture(L"Icons/playpause.png");
  p.plpausb->setImage(im);
  p.plpausb->setPressedImage(im);
  p.recb = p.guienv->addButton(rect<s32>((LOWERMENUWIDTH)/2+10,40,(LOWERMENUWIDTH)/2+LOWERMENUHEIGHT-70,LOWERMENUHEIGHT-40), p.lowermenu, GUIRecord,
                               L"", L"Start or stop recording data.");
  im = p.driver->getTexture(L"Icons/rec.png");
  p.recb->setImage(im);
  p.recb->setPressedImage(im);
  p.recb->setIsPushButton(true);
  p.recb->setPressed(false);

  p.lowermenutime = p.guienv->addStaticText(L"", core::rect<s32>((LOWERMENUWIDTH)/2 - LOWERMENUHEIGHT+70, 130,(LOWERMENUWIDTH)/2+LOWERMENUHEIGHT-70, 150),
                                            true, true, p.lowermenu,GUIText);
  p.lowermenutime->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT);

  p.guienv->addStaticText(L"Number of recordings:", core::rect<s32>((LOWERMENUWIDTH)/2+LOWERMENUHEIGHT-50, 15, LOWERMENUWIDTH-10, 35), false, false, p.lowermenu, GUIText);
  p.guienv->addStaticText(L"Frames per recording:", core::rect<s32>((LOWERMENUWIDTH)/2+LOWERMENUHEIGHT-50, 75, LOWERMENUWIDTH-10, 95), false, false, p.lowermenu, GUIText);

  p.recordnum = p.guienv->addEditBox(L"", core::rect<s32>((LOWERMENUWIDTH)/2+LOWERMENUHEIGHT-50, 40, LOWERMENUWIDTH-10, 60), true, p.lowermenu, GUIRecordNum);
  p.recordint = p.guienv->addEditBox(L"", core::rect<s32>((LOWERMENUWIDTH)/2+LOWERMENUHEIGHT-50, 100, LOWERMENUWIDTH-10, 120), true, p.lowermenu, GUIRecordInterval);

  p.guienv->addStaticText(L"Temporal scale:", core::rect<s32>(10,5,(LOWERMENUWIDTH)/2 - LOWERMENUHEIGHT+50, 25), false, false, p.lowermenu, GUIText);

  p.timescroll = p.guienv->addScrollBar(true,core::rect<s32>(10,30,(LOWERMENUWIDTH)/2 - LOWERMENUHEIGHT+50, 50), p.lowermenu, GUIScrollTime);
  p.timescroll->setMin(-10);
  p.timescroll->setMax(10010);
  p.timescroll->setSmallStep(10);
  p.timescroll->setLargeStep(100);

  p.timedit = p.guienv->addEditBox(L"",core::rect<s32>(10,55,(LOWERMENUWIDTH)/2 - LOWERMENUHEIGHT+50, 75), true, p.lowermenu, GUIEditTime);

  p.guienv->addStaticText(L"Spatial scale:", core::rect<s32>(10,80,(LOWERMENUWIDTH)/2 - LOWERMENUHEIGHT+50, 100), false, false, p.lowermenu, GUIText);

  p.spacescroll = p.guienv->addScrollBar(true,core::rect<s32>(10,105,(LOWERMENUWIDTH)/2 - LOWERMENUHEIGHT+50, 125), p.lowermenu, GUIScrollSpace);
  p.spacescroll->setMin(-10);
  p.spacescroll->setMax(10010);
  p.spacescroll->setSmallStep(10);
  p.spacescroll->setLargeStep(100);

  p.spacedit = p.guienv->addEditBox(L"",core::rect<s32>(10,130,(LOWERMENUWIDTH)/2 - LOWERMENUHEIGHT+50, 150), true, p.lowermenu, GUIEditSpace);

  p.hide_lower_menu();

  p.opendialog = nullptr;
  p.helpwindow = nullptr;
  p.optionwindow = nullptr;
  p.simpareditor = nullptr;
  p.chargeditor = nullptr;
  p.cameditor = nullptr;
  p.keywindow = nullptr;
  p.creditwindow = nullptr;
  p.recordwindow = nullptr;
  p.qedit_color = nullptr;
  p.simpareditbgcolor = nullptr;
  p.simpareditbndcolor = nullptr;

  p.mousemenu = nullptr;

  p.interpreter.add_CGS_units();
  p.interpreter.add_unit(L"º",
                         SII::SI_value<FLType, std::wstring, int>(M_PI/180,
                                                             SII::SI_type<std::wstring, int>(0)));
}


#undef CHECKEY
#define CHECKEY(x) receiver.keyact[p.keysettings[x]]
//Redefinição da verificação das teclas,
//para o caso em que tenha um EventReceiver receiver...

#undef UPDATE_UNFOCUSED
#define UPDATE_UNFOCUSED(Element, Function) if(!p.guienv->hasFocus(Element)) { (Element)->Function; }

void update_gui(props &p, EventReceiver &receiver)
//Actualiza as posições e as indicações do GUI.
{
  size_t i;
  if(p.selected > -1)
    {
      p.arrow[0]->setPosition(p.simarr[p.selected].node->getPosition() + vector3df(p.simarr[p.selected].r/p.scale,0,0));
      p.arrow[1]->setPosition(p.simarr[p.selected].node->getPosition() + vector3df(0, p.simarr[p.selected].r/p.scale,0));
      p.arrow[2]->setPosition(p.simarr[p.selected].node->getPosition() + vector3df(0,0, p.simarr[p.selected].r/p.scale));
      p.arrow[3]->setPosition(p.simarr[p.selected].node->getPosition() + vector3df(-p.simarr[p.selected].r/p.scale,0,0));
      p.arrow[4]->setPosition(p.simarr[p.selected].node->getPosition() + vector3df(0,-p.simarr[p.selected].r/p.scale,0));
      p.arrow[5]->setPosition(p.simarr[p.selected].node->getPosition() + vector3df(0,0,-p.simarr[p.selected].r/p.scale));
    }

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  p.menu->getSubMenu(MenuFile-MenuFile)->setItemEnabled(MenuFSave, p.changed);
  p.menu->getSubMenu(MenuOptions-MenuFile)->setItemEnabled(MenuOSave, p.changedsettings);

  if(p.chargeditor != nullptr)
    {
      if(p.qedit_cb->getSelected() > 0)
        {
          charge &q = p.simarr[p.qedit_cb->getSelected()-1];
          std::wstringstream s;
          for (i = 0; i < 3; ++i)
            {
              s << std::setprecision(std::numeric_limits<FLType>::digits10) << q.pos[i];
              UPDATE_UNFOCUSED(p.qedit_pos[i],setText(s.str().c_str()));
              s.str(std::wstring());
            }
          for (i = 0; i < 3; ++i)
            {
              s << std::setprecision(std::numeric_limits<FLType>::digits10) << q.vel[i];
              UPDATE_UNFOCUSED(p.qedit_vel[i],setText(s.str().c_str()));
              s.str(std::wstring());
            }
          for (i = 0; i < 3; ++i)
            {
              s << std::setprecision(std::numeric_limits<FLType>::digits10) << q.v_to_p(p.lightspeed)[i];
              UPDATE_UNFOCUSED(p.qedit_mom[i],setText(s.str().c_str()));
              s.str(std::wstring());
            }
        }

    }

  p.lowermenu->setRelativePosition(core::rect<s32>((p.windowsize.Width-LOWERMENUWIDTH)/2, p.windowsize.Height-20-LOWERMENUHEIGHT, (p.windowsize.Width-LOWERMENUWIDTH)/2+LOWERMENUWIDTH, p.windowsize.Height-20));
  static int lowmenucount;
  if(p.lowermenu->getRelativePosition().isPointInside(vector2d<s32>(receiver.mouse.X, receiver.mouse.Y)))
    {
      lowmenucount = 0;
      p.show_lower_menu();
      if(!receiver.mouse.isLeftPressed())
        {
          p.past_scale = p.scale;
          p.past_timescale = p.timescale;
          p.spacescroll->setPos(5000);
          p.timescroll->setPos(5000);
        }
    }
  else if(p.lowermenu->isVisible() && (p.opendialog == nullptr || p.opendialog->getID() != GUIDataSaveDialog)
          && (!p.guienv->getFocus() || p.guienv->getFocus()->getParent() != p.lowermenu))
    {
      ++lowmenucount;
      if(lowmenucount > 30)
        {
          p.hide_lower_menu();
          lowmenucount = -1;
          p.blockinteract = false;
        }
    }
  p.lowermenutime->setText((std::wstring(L"t = ")+std::to_wstring(p.timetotal)+L" s").c_str());

  p.menu->getSubMenu(MenuView-MenuFile)->setItemChecked(MenuVPlayPause-MenuVCam, p.play);

  //Seria potencialmente mais eficiente
  //fazer, também, a transição daqui para
  //um qualquer sistema de actualização não imediato,
  //à semelhança dos restantes elementos do GUI,
  //mas torna-se um pouco complicado,
  //pela natureza das scroll bars,
  //pôr tudo isso a funcionar devidamente.
  if(!p.guienv->hasFocus(p.spacedit))
    {
      std::wstringstream s;
      s << std::scientific << p.scale;
      p.spacedit->setText(s.str().c_str());
    }
  if(!p.guienv->hasFocus(p.timedit))
    {
      std::wstringstream s;
      s << std::scientific << p.timescale;
      p.timedit->setText(s.str().c_str());
    }


  if(p.cameditor != nullptr)
    {
      vector3df tempv3f;
      if(!receiver.mouse.isLeftPressed())
        {
          p.past_scale = p.scale;
          p.cameditspacescroll->setPos(5000);
        }
      if(!p.guienv->hasFocus(p.cameditspacedit))
        {
          std::wstringstream s;
          s << std::scientific << p.scale;
          p.cameditspacedit->setText(s.str().c_str());
        }
      std::wstringstream s;
      for (i = 0; i < 3; ++i)
        {
          s << std::setprecision(std::numeric_limits<FLType>::digits10) << getv3df(p.c_lpos,i);
          UPDATE_UNFOCUSED(p.cameditpos[i],setText(s.str().c_str()));
          s.str(std::wstring());
        }
      for (i = 0; i < 3; ++i)
        {
          s << std::setprecision(std::numeric_limits<FLType>::digits10) << getv3df(p.c_lvel,i);
          UPDATE_UNFOCUSED(p.cameditvel[i],setText(s.str().c_str()));
          s.str(std::wstring());
        }
      for (i = 0; i < 3; ++i)
        {
          s << std::setprecision(std::numeric_limits<FLType>::digits10) << getv3df(p.c_lacc,i);
          UPDATE_UNFOCUSED(p.cameditacc[i],setText(s.str().c_str()));
          s.str(std::wstring());
        }
      p.rot.toEuler(tempv3f);
      for (i = 0; i < 3; ++i)
        {
          s << std::setprecision(std::numeric_limits<FLType>::digits10) << getv3df(tempv3f,i)*180/M_PI << L"º";
          UPDATE_UNFOCUSED(p.cameditapos[i],setText(s.str().c_str()));
          s.str(std::wstring());
        }
      for (i = 0; i < 3; ++i)
        {
          s << std::setprecision(std::numeric_limits<FLType>::digits10) << getv3df(p.c_avel,i)*180/M_PI << L"º";
          UPDATE_UNFOCUSED(p.cameditavel[i],setText(s.str().c_str()));
          s.str(std::wstring());
        }
      for (i = 0; i < 3; ++i)
        {
          s << std::setprecision(std::numeric_limits<FLType>::digits10) << getv3df(p.c_aacc,i)*180/M_PI << L"º";
          UPDATE_UNFOCUSED(p.cameditaacc[i],setText(s.str().c_str()));
          s.str(std::wstring());
        }
    }

  if(p.simpareditor != nullptr)
    {
      if(!receiver.mouse.isLeftPressed())
        {
          p.past_timescale = p.timescale;
          p.simparedittimescroll->setPos(5000);
        }
      if(!p.guienv->hasFocus(p.simparedittimedit))
        {
          std::wstringstream s;
          s << std::scientific << p.timescale;
          p.simparedittimedit->setText(s.str().c_str());
        }
    }


  if(p.optionwindow != nullptr)
    {
      if(!receiver.mouse.isLeftPressed())
        {
          p.past_roc = p.rate_of_change;
          p.opt_ROCscroll->setPos(5000);
        }
      UPDATE_UNFOCUSED(p.opt_ROCedit,setText(std::to_wstring(p.rate_of_change).c_str()));

    }

  UPDATE_UNFOCUSED(p.recordnum,setText(std::to_wstring(p.totalprint).c_str()));
  UPDATE_UNFOCUSED(p.recordint,setText(std::to_wstring(p.printerval).c_str()));
}

#undef TEXTWIDTH
#undef TEXTSEP
#undef ELSEP
#undef EBOXWIDTH
#undef UPDATE_DISPLAY

/**********************************

 MOVIMENTO DA CÂMARA E TECLADO

***********************************/

#define VECX (p.relative ? p.sidev : core::vector3df(1,0,0))

#define VECY (p.relative ? p.upv : core::vector3df(0,1,0))

#define VECZ (p.relative ? p.lookatv : core::vector3df(0,0,1))
//Só para facilitar a escrita...


void handle_free_motion(props &p, EventReceiver &receiver)
//Reconhece os inputs do teclado relativamente ao movimento da câmara.
{
  core::quaternion q;
#define ROTADJUST 0.5f
#define TRANSADJUST (10.0f)
  //Factores de ajuste...
  if(CHECKEY(Angle))
    {
      if(CHECKEY(Forward))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_avel.X -= (1 - 2*p.invertx) * p.rate_of_change * p.deltat * ROTADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_aacc.X -= (1 - 2*p.invertx) * p.rate_of_change * p.deltat * ROTADJUST;
            }
          else
            {
              q.fromAngleAxis(-(1 - 2*p.invertx) * p.rate_of_change * p.deltat * ROTADJUST, VECX);
              p.rot = p.rot*q;
              p.upv = q*p.upv;
              p.lookatv = q*p.lookatv;
              p.sidev = q*p.sidev;
            }
          p.changed = true;
        }
      if(CHECKEY(Backward))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_avel.X += (1 - 2*p.invertx) * p.rate_of_change * p.deltat * ROTADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_aacc.X += (1 - 2*p.invertx) * p.rate_of_change * p.deltat * ROTADJUST;
            }
          else
            {
              q.fromAngleAxis((1 - 2*p.invertx) * p.rate_of_change * p.deltat * ROTADJUST, VECX);
              p.rot = p.rot*q;
              p.upv = q*p.upv;
              p.lookatv = q*p.lookatv;
              p.sidev = q*p.sidev;
            }
          p.changed = true;
        }
      if(CHECKEY(Left))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_avel.Y -= p.rate_of_change * p.deltat * ROTADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_aacc.Y -= p.rate_of_change * p.deltat * ROTADJUST;
            }
          else
            {
              q.fromAngleAxis(-p.rate_of_change * p.deltat * ROTADJUST, VECY);
              p.rot = p.rot*q;
              p.upv = q*p.upv;
              p.lookatv = q*p.lookatv;
              p.sidev = q*p.sidev;
            }
          p.changed = true;
        }
      if(CHECKEY(Right))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_avel.Y += p.rate_of_change * p.deltat * ROTADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_aacc.Y += p.rate_of_change * p.deltat * ROTADJUST;
            }
          else
            {
              q.fromAngleAxis(p.rate_of_change * p.deltat * ROTADJUST, VECY);
              p.rot = p.rot*q;
              p.upv = q*p.upv;
              p.lookatv = q*p.lookatv;
              p.sidev = q*p.sidev;
            }
          p.changed = true;
        }
      if(CHECKEY(Up))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_avel.Z -= (1 - 2*p.inverty) * p.rate_of_change * p.deltat * ROTADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_aacc.Z -= (1 - 2*p.inverty) * p.rate_of_change * p.deltat * ROTADJUST;
            }
          else
            {
              q.fromAngleAxis(-(1 - 2*p.inverty) * p.rate_of_change * p.deltat * ROTADJUST, VECZ);
              p.rot = p.rot*q;
              p.upv = q*p.upv;
              p.lookatv = q*p.lookatv;
              p.sidev = q*p.sidev;
            }
          p.changed = true;
        }
      if(CHECKEY(Down))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_avel.Z += (1 - 2*p.inverty) * p.rate_of_change * p.deltat * ROTADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_aacc.Z += (1 - 2*p.inverty) * p.rate_of_change * p.deltat * ROTADJUST;
            }
          else
            {
              q.fromAngleAxis((1 - 2*p.inverty) * p.rate_of_change * p.deltat * ROTADJUST, VECZ);
              p.rot = p.rot*q;
              p.upv = q*p.upv;
              p.lookatv = q*p.lookatv;
              p.sidev = q*p.sidev;
            }
          p.changed = true;
        }
    }
  else
    {
      if(CHECKEY(Forward))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_lvel += VECZ*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_lacc += VECZ*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else
            {
              p.c_lpos += VECZ*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          p.changed = true;
        }
      if(CHECKEY(Backward))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_lvel -= VECZ*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_lacc -= VECZ*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else
            {
              p.c_lpos -= VECZ*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          p.changed = true;
        }
      if(CHECKEY(Left))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_lvel -= VECX*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_lacc -= VECX*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else
            {
              p.c_lpos -= VECX*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          p.changed = true;
        }
      if(CHECKEY(Right))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_lvel += VECX*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_lacc += VECX*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else
            {
              p.c_lpos += VECX*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          p.changed = true;
        }
      if(CHECKEY(Up))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_lvel += VECY*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_lacc += VECY*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else
            {
              p.c_lpos += VECY*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          p.changed = true;
        }
      if(CHECKEY(Down))
        {
          if(CHECKEY(Velocity) && !CHECKEY(Acceleration))
            {
              p.c_lvel -= VECY*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else if(CHECKEY(Acceleration))
            {
              p.c_lacc -= VECY*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          else
            {
              p.c_lpos -= VECY*p.rate_of_change*p.deltat*TRANSADJUST;
            }
          p.changed = true;
        }
    }
}

void handle_keys(props &p, EventReceiver &receiver)
//Reconhece os inputs do teclado.
{
  static bool didonce;
  if(didonce)
    {
      if (!receiver.key.PressedDown)
        {
          didonce = false;
        }
      return;
    }
  if (p.blockinteract || p.opendialog != nullptr)
    {
      return;
    }
  handle_free_motion(p, receiver);
  if(CHECKEY(Reset))
    {
      p.lookatv.set(0,0,1);
      p.upv.set(0,1,0);
      p.sidev.set(1,0,0);
      p.rot.set(0,0,0);
      p.changed = true;
    }
  if(CHECKEY(BackToOrigin))
    {
      p.c_lpos = vector3df(0,0,0);
      p.changed = true;
    }
  if(CHECKEY(Stop))
    {
      p.c_avel = p.c_aacc = p.c_lvel = p.c_lacc = vector3df(0,0,0);
      p.changed = true;
    }
  if(CHECKEY(RelativeSwitch) && receiver.key.PressedDown)
    {
      didonce = true;
      p.relative = !p.relative;
      if(p.optionwindow != nullptr)
        {
          p.opt_relmov->setChecked(p.relative);
        }
      p.changedsettings = true;
    }
  if(CHECKEY(PlayPause) && receiver.key.PressedDown)
    {
      didonce = true;
      p.play = !p.play;
    }
}

void handle_mouse(props &p, EventReceiver &receiver)
//Tenta reconhecer os inputs do rato.
{
  static irr::s32 which;
  static irr::core::vector3d<f32> initial_dir;
  if(p.toolinuse != ToolbarMove-ToolbarMove || receiver.mouse.Event == EMIE_LMOUSE_LEFT_UP || !receiver.mouse.isLeftPressed() || p.selected < 0)
    {
      which = -1;
    }
  if (p.blockinteract || p.opendialog != nullptr)
    {
      which = -1;
      return;
    }
#define WHEELADJUST 0.01
  //Para ajustar a sensibilidade da rodinha do rato...
  if(receiver.mouse.Event == EMIE_MOUSE_WHEEL)
    {
      if(CHECKEY(Angle))
        {
          if(receiver.mouse.Wheel < 0)
            {
              p.timescale *= 1.0 + (receiver.mouse.Wheel * WHEELADJUST * (CHECKEY(Velocity) ? p.rate_of_change : 1.0f));
              p.changed = true;
            }
          else if(receiver.mouse.Wheel > 0)
            {
              p.timescale /= 1.0 - (receiver.mouse.Wheel * WHEELADJUST * (CHECKEY(Velocity) ? p.rate_of_change : 1.0f));
              p.changed = true;
            }
        }
      else
        {
          if(receiver.mouse.Wheel > 0)
            {
              p.scale *= 1.0 + (receiver.mouse.Wheel * WHEELADJUST * (CHECKEY(Velocity) ? p.rate_of_change : 1.0f));
              p.changed = true;
            }
          else if(receiver.mouse.Wheel < 0)
            {
              p.scale /= 1.0 - (receiver.mouse.Wheel * WHEELADJUST * (CHECKEY(Velocity) ? p.rate_of_change : 1.0f));
              p.changed = true;
            }
        }
      receiver.mouse.Event = EMIE_COUNT;
      receiver.mouse.Wheel = 0.0f;
    }
  if(receiver.mouse.Event == EMIE_RMOUSE_PRESSED_DOWN)
    {
      p.mmenusel = ( p.smgr->getSceneCollisionManager()
                     ->getSceneNodeFromScreenCoordinatesBB(
                                                           core::vector2di( p.windowsize.Width*receiver.mouse.X/(p.windowsize.Width - HTABSIZE),
                                                                            p.windowsize.Height*receiver.mouse.Y/(p.windowsize.Height - VTABSIZE) ),
                                                           0, true  )   )->getID();
      if(p.mmenusel > -1)
        {
          p.create_mousemenu();
          p.mousemenu->setRelativePosition(core::vector2di(receiver.mouse.X, receiver.mouse.Y));
          p.mousemenu->setItemText(0,p.simarr[p.mmenusel].label.c_str());
          p.mousemenu->setItemEnabled(0,false);
        }
    }
  if(p.toolinuse > -1 && receiver.mouse.Event == EMIE_LMOUSE_PRESSED_DOWN)
    {
      irr::core::line3d<f32> ray = p.smgr->getSceneCollisionManager()->
        getRayFromScreenCoordinates(core::position2di(receiver.mouse.X, receiver.mouse.Y),
                                    p.camera);
      float tempfl;
      which = p.smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(ray, 0, true)->getID();
      receiver.mouse.Event = EMIE_COUNT;
      charge newq;
      switch(p.toolinuse+ToolbarMove)
        {
        case ToolbarMove:
          if(which > -1)
            {
              p.change_selected(which);
            }
          else if (p.selected > -1 && which >= GUIArrow1 && which <= GUIArrow6)
            {
              which -= GUIArrow1;
              initial_dir = ray.getVector().normalize();
            }
          break;
        case ToolbarEdit:
          {
            if(which > -1)
              {
                if(p.chargeditor == nullptr)
                  {
                    p.create_chargeditor();
                    p.chargedit_select(which+1);
                    p.menu->getSubMenu(MenuEdit - MenuFile)->setItemChecked(MenuECharges-MenuESimPar,true);
                  }
                else
                  {
                    p.chargedit_select(which+1);
                  }
                if(!CHECKEY(Angle))
                  {
                    p.toolinuse = -1;
                    p.toolbarbuttons[ToolbarEdit-ToolbarMove]->setPressed(false);
                  }
              }
            break;
          }
        case ToolbarStop:
          if(which > -1)
            {
              p.simarr[which].stop(p.deltat*p.timescale);
              p.changed = true;
              if(!CHECKEY(Angle))
                {
                  p.toolinuse = -1;
                  p.toolbarbuttons[ToolbarStop-ToolbarMove]->setPressed(false);
                }
            }
          break;
        case ToolbarAdd:
          tempfl = std::min({fabs(p.c_lpos.X-p.limits[0]),
                             fabs(p.c_lpos.Y-p.limits[1]),
                             fabs(p.c_lpos.Z-p.limits[2]),
                             fabs(p.c_lpos.X-p.limits[3]),
                             fabs(p.c_lpos.Y-p.limits[4]),
                             fabs(p.c_lpos.Z-p.limits[5]),});
          newq = charge( irr_to_ours(ray.start + ray.getVector().normalize()*p.scale
                                     *(tempfl*((FLType) rand())/((FLType) RAND_MAX))),
                         L"Charge #", true);
          newq.col.set(255, rand()%255, rand()%255, rand()%255);
          newq.label += std::to_wstring(p.simarr.size());
          p.add_charge(newq);
          if(!CHECKEY(Angle))
            {
              p.toolinuse = -1;
              p.toolbarbuttons[ToolbarAdd-ToolbarMove]->setPressed(false);
            }
          p.changed = true;
          break;
        case ToolbarClone:
          if(which > -1)
            {
              charge tempq(p.simarr[which]);
              tempq.reset_position(p);
              p.add_charge(tempq);
              if(!CHECKEY(Angle))
                {
                  p.toolinuse = -1;
                  p.toolbarbuttons[ToolbarClone-ToolbarMove]->setPressed(false);
                }
              p.changed = true;
            }
          break;
        case ToolbarDelete:
          if(which > -1)
            {
              p.delete_charge(which);
              if(!CHECKEY(Angle))
                {
                  p.toolinuse = -1;
                  p.toolbarbuttons[ToolbarDelete-ToolbarMove]->setPressed(false);
                }
              p.changed = true;
            }
          break;
        default:
          break;
        }
    }
#define DRAGADJUST 10.0f
  //Factor de ajuste do arrastamento via setinhas.
  if(p.toolinuse == ToolbarMove - ToolbarMove && which > -1 && which < 6 && receiver.mouse.isLeftPressed())
    {
      core::line3d<f32> temp = p.smgr->getSceneCollisionManager()->
        getRayFromScreenCoordinates(core::position2di(receiver.mouse.X, receiver.mouse.Y),
                                    p.camera);
      vector3df vect = temp.getVector().normalize() - initial_dir;
      irr::s32 id = -1;
      if(p.smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(temp, 0, true, p.simarr[p.selected].node) != nullptr)
        {
          id = p.smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(temp, 0, true, p.simarr[p.selected].node)->getID();
        }
      if(id == which + GUIArrow1)
        {
          initial_dir = temp.getVector().normalize();
        }
      else
        {
          switch(which)
            {
            case 0:
              p.simarr[p.selected].pos[0] += DRAGADJUST*p.deltat*p.rate_of_change*
                vect.dotProduct(vector3df(1,0,0));
              break;
            case 1:
              p.simarr[p.selected].pos[1] += DRAGADJUST*p.deltat*p.rate_of_change*
                vect.dotProduct(vector3df(0,1,0));
              break;
            case 2:
              p.simarr[p.selected].pos[2] += DRAGADJUST*p.deltat*p.rate_of_change*
                vect.dotProduct(vector3df(0,0,1));
              break;
            case 3:
              p.simarr[p.selected].pos[0] -= DRAGADJUST*p.deltat*p.rate_of_change*
                vect.dotProduct(vector3df(-1,0,0));
              break;
            case 4:
              p.simarr[p.selected].pos[1] -= DRAGADJUST*p.deltat*p.rate_of_change*
                vect.dotProduct(vector3df(0,-1,0));
              break;
            case 5:
              p.simarr[p.selected].pos[2] -= DRAGADJUST*p.deltat*p.rate_of_change*
                vect.dotProduct(vector3df(0,0,-1));
              break;
            default:
              break;
            }
          p.changed = true;
        }
    }
}

void cam_evolve (props &p)
//Efectua a evolução da posição e orientação
//da câmara ao longo do tempo.
{
  core::quaternion q;

  p.c_lpos += p.c_lvel*p.deltat;

  p.c_lvel += p.c_lacc*p.deltat;

  q.fromAngleAxis(p.deltat*p.c_avel.X, VECX);
  p.upv = q*p.upv;
  p.lookatv = q*p.lookatv;
  p.sidev = q*p.sidev;
  p.rot = p.rot*q;

  q.fromAngleAxis(p.deltat*p.c_avel.Y, VECY);
  p.upv = q*p.upv;
  p.lookatv = q*p.lookatv;
  p.sidev = q*p.sidev;
  p.rot = p.rot*q;

  q.fromAngleAxis(p.deltat*p.c_avel.Z, VECZ);
  p.upv = q*p.upv;
  p.lookatv = q*p.lookatv;
  p.sidev = q*p.sidev;
  p.rot = p.rot*q;

  p.c_avel += p.c_aacc*p.deltat;
}

/**********************************

 IMPORTANTÍSSIMO: EVOLUÇÃO TEMPORAL!

***********************************/
void charge_evolution(props &p, const FLType dt, const size_t first, const size_t last)
//O std::vector no fim guarda o array dos valores de campo eléctrico
{
  std::pair<fspoint<FLType>,fspoint<FLType>> tempfields;

  const FLType w0 = -cbrt(2.0)/(2-cbrt(2.0)),
    w1 = 1/(2-cbrt(2.0)),
    c14 = 0.5*w1,
    c23 = 0.5*(w0+w1),
    d13 = w1,
    d2 = w0;
  fspoint<FLType> tempos, tempvel, tempe(0.0);
#pragma omp parallel for
  for (size_t i = first; i < last; ++i)
    //Usa-se uma integração de Yoshida...
    {
      //1
      tempos = p.simarr[i].pos+p.simarr[i].vel*c14*dt;
      tempfields = calculate_fields(p, tempos, p.timetotal + d13*dt, i);
      tempvel = p.simarr[i].vel+p.simarr[i].acceleration(tempfields, p)*d13*dt;
      tempe += tempfields.first;
      //2
      tempos += tempvel*c23*dt;
      tempfields = calculate_fields(p, tempos, p.timetotal + d13*dt + c23*dt, i);
      tempvel += p.simarr[i].acceleration(tempfields, p)*d2*dt;
      tempe += tempfields.first;

      //3
      tempos += tempvel*c23*dt;
      tempfields = calculate_fields(p, tempos, p.timetotal + d13*dt + 2*c23*dt, i);
      tempvel += p.simarr[i].acceleration(tempfields, p)*d13*dt;
      tempe += tempfields.first;

      //Finalizar

      p.simarr[i].pos = tempos+tempvel*c14*dt;

      p.simarr[i].acc = (tempvel-p.simarr[i].vel)/dt;

      p.simarr[i].vel = tempvel;

      p.temp_earr[i] = tempe/3;
    }
}

void update_history(props &p)
{
  p.changed = true;
  size_t i;
  charge_rec *forhist = new charge_rec[p.simarr.size()];

  for (i = 0; i < p.simarr.size(); ++i)
    {
      //Ciclo de novo registo
      forhist[i] = charge_rec(p.simarr[i]);
      forhist[i].time = p.timetotal;
    }

  p.hist.record(forhist);

  delete[] forhist;
}

size_t temporal_evolve(props &p, const FLType dt)
//Calcula a evolução temporal das partículas
//(é, efectivamente, a função de simulação numérica...)
//O earr serve para aglomerar os valores de campo eléctrico,
//sendo usado mais tarde na representação gráfica das cargas.
{
  size_t i, j, count = 0;
  //Por uma questão de estatística (e de curiosidade),
  //conta-se o número de colisões ocorridas...

  if(dt != 0)
    {
      update_history(p);
      charge_evolution(p,dt,0,p.simarr.size());
    }
  else
    {
#pragma omp parallel for
      for(size_t k = 0; k < p.simarr.size(); ++k)
        {
          if(p.simarr[k].deformable)
            {
              p.temp_earr[k] = calculate_fields(p, p.simarr[k].pos, p.timetotal, k).first;
            }
        }
      if(!p.recb->isPressed() && p.timetotal > 10000*p.timescale)
        {
          reset_history_times(p);
        }
    }

  for (i = 0; i < p.simarr.size(); ++i)
    //Ciclo de determinação das colisões...
    {
      for(j = i; j < p.simarr.size(); ++j)
        {
          count += (size_t) p.simarr[i].process_collision(p.simarr[j], p.lightspeed);
          //Verifica-se se ocorre uma colisão, em caso afirmativo,
          //reage-se e incrementa-se o contador.
        }
      for (j = 0; j < 3; ++j)
        {
          if (p.simarr[i].pos[j] < p.limits[j] + p.simarr[i].r)
            {
              p.simarr[i].pos[j] = p.limits[j] + p.simarr[i].r;
              p.simarr[i].vel[j] = fabs(p.simarr[i].vel[j]);
              p.simarr[i].acc.set_all(0);
            }
          if (p.simarr[i].pos[j] > p.limits[j+3] - p.simarr[i].r)
            {
              p.simarr[i].pos[j] = p.limits[j+3] - p.simarr[i].r;
              p.simarr[i].vel[j] = -fabs(p.simarr[i].vel[j]);
              p.simarr[i].acc.set_all(0);
            }
        }
    }
  p.timetotal += dt;
  return count;
}

/**********************************

 Diversidades diversas

***********************************/

void commandline_askfile(props &p)
//Permite perguntar, através da linha de comandos,
//o ficheiro de que se irá ler a configuração.
{
  std::string s;
  std::cout << "Please, enter the file from which the configuration should be loaded. " << std::flush;
  std::cin.clear();
  std::cin.sync();
  std::getline(std::cin,s);
  load_config(s.c_str(), p);
}

void commandline_input(props &p)
//Permite fazer o input de uma configuração
//através da linha de comandos.
{
  size_t i, num;
  std::cout << "Please, enter the number of charges to be simulated. " << std::flush;
  std::cin >> num;
  while(std::cin.fail())
    {
      std::cerr << "Please enter a valid, non-negative integer. " << std::flush;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cin >> num;
    }
  p.simarr.resize(num);
  std::cout << "Please, enter the information regarding the charges, in the form:\n"
            << "<Position> <Velocity> <Acceleration> <mass> <charge> <radius> <label>\n\n"
            << "(The three first as 3-dimensional vectors, that is, as a set of three reals.)" << std::endl;
  for(i = 1; i <= num; ++i)
    {
      std::cout << "\nCharge #" << i << ": " << std::flush;
      std::cin >> p.simarr[i-1];
      if(std::cin.fail())
        {
          std::cerr << "Invalid information! Please, try again.\n" << std::endl;
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          --i;
        }
    }
  std::cout << "Now, enter the limits of the simulation area, as a 6-dimensional vector, with "
            << "the first three terms being the lower limits and the second being the upper.\n" << std::endl;
  std::cin >> p.limits[0] >> p.limits[1] >> p.limits[2] >> p.limits[3] >> p.limits[4] >> p.limits[5];
  while (std::cin.fail() || p.limits[0] > p.limits[3] || p.limits[1] > p.limits[4] || p.limits[2] > p.limits[5])
    {
      std::cerr << "The information that was entered was invalid. Please, try again.\n" << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cin >> p.limits[0] >> p.limits[1] >> p.limits[2] >> p.limits[3] >> p.limits[4] >> p.limits[5];
    }
  std::cout << "Enter the external electric and magnetic fields as two sets of mathematical expressions.\n" << std::endl;
  getline(std::cin,p.Eext[0]);
  getline(std::cin,p.Eext[1]);
  getline(std::cin,p.Eext[2]);
  getline(std::cin,p.Bext[0]);
  getline(std::cin,p.Bext[1]);
  getline(std::cin,p.Bext[2]);
  p.Efunc[0].Parse(p.Eext[0],"x,y,z,t");
  p.Efunc[0].Optimize();
  p.Efunc[1].Parse(p.Eext[1],"x,y,z,t");
  p.Efunc[1].Optimize();
  p.Efunc[2].Parse(p.Eext[2],"x,y,z,t");
  p.Efunc[2].Optimize();
  p.Bfunc[0].Parse(p.Bext[0],"x,y,z,t");
  p.Bfunc[0].Optimize();
  p.Bfunc[1].Parse(p.Bext[1],"x,y,z,t");
  p.Bfunc[1].Optimize();
  p.Bfunc[2].Parse(p.Bext[2],"x,y,z,t");
  p.Bfunc[2].Optimize();
  std::cout << "Finally, enter the value of the speed of light during the simulation. " << std::flush;
  std::cin >> p.lightspeed;
  while (std::cin.fail())
    {
      std::cerr << "Invalid information! Please enter a valid real number. " << std::flush;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cin >> p.lightspeed;
    }
  std::cout << "And it's done!\n\nShould this configuration be saved in a file, for later use (y to accept)? " << std::flush;
  char b;
  std::cin >> b;
  if(b == 'y' || b == 'Y')
    {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "\nPlease insert the file in which the configuration should be saved. ";
      std::cin.clear();
      std::cin.sync();
      std::string s;
      std::getline(std::cin, s);
      p.hist.rearrange();
      save_simple_config(s.c_str(), p);
    }
}

/**********************************

 A MAIN!

***********************************/

int main(int argc, char **argv)
{
  bool graph = false, simulate = false;
  int i, loadindex = 0, saveindex = 0;
  size_t j, k, collisions = 0;
  srand(time(NULL));
  FLType timestep = -1.0;
  std::string s;

  props p;
  p.printerval = 0;
  p.totalprint = 0;

  std::setlocale(LC_ALL, "pt_PT.UTF-8");
  //Por uma questão de validade do output.

  for(i=1; i < argc; ++i)
    {
      if( std::strcmp(argv[i], "-gr") == 0 || std::strcmp(argv[i], "-graph")== 0 ||
          std::strcmp(argv[i], "-num")== 0|| std::strcmp(argv[i], "-numeric")== 0   )
        {
          graph = true;
        }
      else if( std::strcmp(argv[i], "-sim") == 0 ||
               std::strcmp(argv[i], "-simulate")== 0 ||
               std::strcmp(argv[i], "-simul")== 0       )
        {
          simulate = true;
        }
      else if( std::strcmp(argv[i], "-l") == 0 ||
               std::strcmp(argv[i], "-load")== 0  )
        {
          if(i < argc-1)
            {
              loadindex = i+1;
              ++i;
              //Uma vez que se considera o próximo argumento o nome do ficheiro,
              //não vale a pena considerá-lo na análise dos restantes argumentos...
            }
        }
      else if( std::strcmp(argv[i], "-s") == 0 ||
               std::strcmp(argv[i], "-save")== 0   )
        {
          if(i < argc-1)
            {
              p.out.open(argv[i+1]);
              saveindex = i+1;
              ++i;
              //Uma vez que se considera o próximo argumento o nome do ficheiro,
              //não vale a pena considerá-lo na análise dos restantes argumentos...
            }
        }
    }
  if(argc == 1)
    {
      simulate = true;
    }
  //Permite-se ao utilizador que diga o que pretende fazer:
  //dar um argumento -gr ou -num ao programa permite fazer o gráfico (ROOT),
  //dar um argumento -sim permite fazer a simulação.
  //Na ausência de argumentos, assume-se que se pretende simular...
  //Um argumento do tipo -l <ficheiro> permite indicar
  //um ficheiro de configuração das esferas para ler.
  //Um argumento do tipo -s <ficheiro> permite indicar
  //um ficheiro onde guardar os resultados para o gráfico.

  if (simulate)
    //Usa-se Irrlicht para mostrar as coisas bonitinhas.
    {

      p.selected = -1;
      p.mmenusel = -1;
      p.toolinuse = -1;

      video::E_DRIVER_TYPE driverType=driverChoiceConsole();
      //Permite-se ao utilizador a escolha das drivers...

      if (driverType==video::EDT_COUNT)
        {
          std::cerr << "Error: Invalid driver selected. Exiting now...\n" << std::endl;
          return 1;
        }

      EventReceiver receiver(p);
      p.device = createDevice(driverType, dimension2d<u32>(720, 720), 32, false, false, true, &receiver);
      //O true é para garantir VSync, isto é, que a framerate não ultrapassa
      //a taxa de actualização do monitor (poupa recursos e garante animação suave).

      if (!p.device)
        {
          std::cerr << "Error: SimEl was unable to create the graphical device. Exiting now...\n" << std::endl;
          return 1;
        }

      p.driver = p.device->getVideoDriver();
      p.smgr = p.device->getSceneManager();
      p.guienv = p.device->getGUIEnvironment();
      p.device->setResizable(true);

      core::stringc program_directory = p.device->getFileSystem()->getAbsolutePath(p.device->getFileSystem()->getWorkingDirectory());

      load_settings(DEFAULT_SETTINGS, p);

      full_load_config((loadindex != 0 ? argv[loadindex] : LAST_CONFIG), p);

      u32 before = p.device->getTimer()->getTime();
      p.smgr->addCameraSceneNode();
      p.camera = p.smgr->getActiveCamera();
      p.play = false;
      for(j = 0; j < 6; ++j)
        {
          p.bounds[j] = p.smgr->addCubeSceneNode(1.0f);
          p.bounds[j]->setMaterialFlag(video::EMF_LIGHTING, false);
          p.bounds[j]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
          p.bounds[j]->setIsDebugObject(true);
          irr::scene::IMeshSceneNode *temp = p.smgr->addCubeSceneNode(1.0f, p.bounds[j]);
          temp->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
          temp->setIsDebugObject(true);
          p.smgr->getMeshManipulator()->setVertexColors(p.bounds[j]->getMesh(), p.boundcolor);
        }

      IMeshSceneNode *node = p.smgr->addSphereSceneNode(1.0, 64);
      p.vertexpos = new vector3df[node->getMesh()->getMeshBuffer(0)->getVertexCount()];
      for (u32 indx = 0; indx < node->getMesh()->getMeshBuffer(0)->getVertexCount(); ++indx)
        {
          p.vertexpos[indx] = node->getMesh()->getMeshBuffer(0)->getPosition(indx);
        }
      node->remove();

      make_gui(p);
      //Adiciona os elementos do GUI...

      bool didonce = false;
      p.blockinteract = false;

      size_t num_frames = 0;
      //Para a contagem dos frames para impressão...

      while(p.device->run())
        {
          p.windowsize = p.driver->getScreenSize();
          if(p.device->isWindowActive())
            {
              u32 after = p.device->getTimer()->getTime();
              p.deltat = ((float)(after - before)) / 1000.f;
              //O tempo é medido em milissegundos...
              before = after;

              if( p.opendialog == nullptr && (p.play || ( CHECKEY(OneStep) && receiver.key.PressedDown && !didonce && !p.blockinteract)) )
                {
                  didonce = true;
                  for(j = 0; j < p.itercount; ++j)
                    {
                      collisions += temporal_evolve(p, p.deltat*p.timescale/p.itercount);
                    }
                  if(p.recb->isPressed() && p.totalprint > 0)
                    {
                      num_frames++;
                      //p.timetotal += p.deltat*p.timescale;
                      if(num_frames >= p.printerval)
                        {
                          num_frames = 0;
                          if(write_data(p.out, p.simarr, p.timetotal, p.lightspeed))
                            {
                              std::cerr << "Error: unable to save data to output file.\n" << std::endl;
                            }
                          p.totalprint--;
                          if(!p.guienv->hasFocus(p.recordnum))
                            {
                              p.recordnum->setText(std::to_wstring(p.totalprint).c_str());
                            }
                          if(!p.guienv->hasFocus(p.recordint))
                            {
                              p.recordint->setText(std::to_wstring(p.printerval).c_str());
                            }
                          p.update_recordwindow();
                        }
                    }
                  else if(p.recb->isPressed())
                    {
                      num_frames = 0;
                      p.recb->setPressed(false);
                      p.menu->getSubMenu(MenuView - MenuFile)->setItemChecked(MenuVRecing-MenuVCam,false);
                      p.out.close();
                      if(!p.guienv->hasFocus(p.recordnum))
                        {
                          p.recordnum->setText(std::to_wstring(p.totalprint).c_str());
                        }
                      if(!p.guienv->hasFocus(p.recordint))
                        {
                          p.recordint->setText(std::to_wstring(p.printerval).c_str());
                        }
                      p.update_recordwindow();
                    }
                }
              else
                {
                  collisions += temporal_evolve(p, 0);
                }
              if(!receiver.key.PressedDown)
                {
                  didonce = false;
                }
              p.camera->setFOV((float) M_PI*0.25f);
              p.camera->setAspectRatio(( (float) p.windowsize.Width-HTABSIZE)/( (float) p.windowsize.Height-VTABSIZE));
              //Ajustar a visualização, para que não haja deformação das cargas...


              p.bounds[0]->setPosition(vector3df(p.limits[0]-0.5,(p.limits[4]+p.limits[1])/2,(p.limits[5]+p.limits[2])/2)/p.scale);
              p.bounds[1]->setPosition(vector3df((p.limits[3]+p.limits[0])/2,p.limits[1]-0.5,(p.limits[5]+p.limits[2])/2)/p.scale);
              p.bounds[2]->setPosition(vector3df((p.limits[3]+p.limits[0])/2,(p.limits[4]+p.limits[1])/2,p.limits[2]-0.5)/p.scale);
              p.bounds[3]->setPosition(vector3df(p.limits[3]+0.5,(p.limits[4]+p.limits[1])/2,(p.limits[5]+p.limits[2])/2)/p.scale);
              p.bounds[4]->setPosition(vector3df((p.limits[3]+p.limits[0])/2,p.limits[4]+0.5,(p.limits[5]+p.limits[2])/2)/p.scale);
              p.bounds[5]->setPosition(vector3df((p.limits[3]+p.limits[0])/2,(p.limits[4]+p.limits[1])/2,p.limits[5]+0.5)/p.scale);

              p.bounds[0]->setScale(vector3df(1.0, p.limits[4]-p.limits[1], p.limits[5]-p.limits[2])/p.scale);
              p.bounds[1]->setScale(vector3df(p.limits[3]-p.limits[0], 1.0, p.limits[5]-p.limits[2])/p.scale);
              p.bounds[2]->setScale(vector3df(p.limits[3]-p.limits[0], p.limits[4]-p.limits[1], 1.0)/p.scale);
              p.bounds[3]->setScale(vector3df(1.0, p.limits[4]-p.limits[1], p.limits[5]-p.limits[2])/p.scale);
              p.bounds[4]->setScale(vector3df(p.limits[3]-p.limits[0], 1.0, p.limits[5]-p.limits[2])/p.scale);
              p.bounds[5]->setScale(vector3df(p.limits[3]-p.limits[0], p.limits[4]-p.limits[1], 1.0)/p.scale);

#pragma omp parallel for
              for(size_t k = 0; k < p.simarr.size(); ++k)
                {
                  p.simarr[k].draw(p, p.temp_earr[k]);
                }

              handle_keys(p, receiver);

              handle_mouse(p, receiver);
              //Reconhecer os inputs do utilizador.

              cam_evolve(p);
              //Evolui a câmara no tempo...

              p.camera->setPosition(p.c_lpos);
              p.camera->setTarget(p.c_lpos + p.lookatv);
              p.camera->setUpVector(p.upv);

              update_gui(p, receiver);

              p.driver->setViewPort(core::rect<s32>(0,0,p.windowsize.Width, p.windowsize.Height));
              p.driver->beginScene(true, true, p.background);
              /*p.driver->setViewPort(core::rect<s32>(0,0,p.windowsize.Width-HTABSIZE, p.windowsize.Height-VTABSIZE));
                p.smgr->drawAll();*/
              //p.driver->setViewPort(core::rect<s32>(0,0,p.windowsize.Width, p.windowsize.Height));
              p.smgr->drawAll();
              p.guienv->drawAll();
              p.driver->endScene();

            }

          else
            {
              before = p.device->getTimer()->getTime();
              p.deltat = 0.0f;
              p.device->yield();
            }
        }
      if(p.out.is_open())
        {
          p.out.close();
        }
      p.device->getFileSystem()->changeWorkingDirectoryTo(program_directory);
      save_aux_config(LAST_CONFIG AUXCONFIG_EXT, p);
      p.device->run();
      p.device->drop();
      delete[] p.vertexpos;
    }
  else if (graph)
    //Faz-se só a simulação numérica...
    {
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      if (loadindex != 0)
        {
          load_config(argv[loadindex], p);
        }
      else
        {
          std::cout << "Welcome to SimCampEl's command line interface.\n\n"
            "This program simulates interactions  between electrical"
            " particles using the relativistic version of the electromagnetic "
            "equations of motion. To do that, it needs to know the configuration"
            " of the particle system.\n\nThere are three ways it can get that information:\n\n"
            "   (1) Load the configuration from a file\n\n   (2) Input the information in the"
            " command line\n\n   Default: Load the configuration that was used "
            "the last time the program was run.\n\nWhat should SimCampEl do? " << std::flush;
          std::getline(std::cin, s);
          if(s.size() && s[0] == '1')
            {
              commandline_askfile(p);
            }
          else if(s.size() && s[0] == '2')
            {
              commandline_input(p);
            }
          else
            {
              load_config(LAST_CONFIG, p);
            }
        }
      while(true)
        {
          std::cin.clear();
          std::cin.sync();
          std::cout << "How many prints do you want to perform? " << std::flush;
          std::cin >> p.totalprint;
          while(std::cin.fail())
            {
              std::cout << "Invalid input! Please, enter a valid non-negative integer. " << std::flush;
              std::cin.clear();
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              std::cin >> p.totalprint;
            }

          std::cout << "And which should be the interval between the recorded points? " << std::flush;
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cin >> p.printerval;
          while(std::cin.fail() && p.printerval < 1)
            {
              std::cout << "Invalid input! Please, enter a valid positive integer. " << std::flush;
              std::cin.clear();
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              std::cin >> p.printerval;
            }

          std::cout << "And what should be the time step? " << std::flush;
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cin >> timestep;
          while(std::cin.fail() || timestep <= 0.0)
            {
              std::cout << "Invalid input! Please, enter a positive real value. " << std::flush;
              std::cin.clear();
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              std::cin >> timestep;
            }

          std::cout << "And how many threads should we use? " << std::flush;
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          std::cin >> p.numthreads;
          while(std::cin.fail())
            {
              std::cout << "Invalid input! Please, enter a valid positive integer. " << std::flush;
              std::cin.clear();
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              std::cin >> timestep;
            }

          s = "\0";
          std::cin.clear();
          std::cin.sync();
          while (!p.out.is_open())
            {
              if (s[0] != '\0' || saveindex != 0)
                {
                  std::cerr << "Error: unable to open the specified output file." << std::endl;
                }
              std::cout << "Please, input a valid file for data output. " << std::flush;
              std::cin.clear();
              std::cin.sync();
              std::getline(std::cin, s);
              p.out.open(s.c_str());
            }
          p.out << p.simarr.size() << "\n";
          for(j = 0; j < p.simarr.size(); ++j)
            {
              p.out << converter.to_bytes(p.simarr[j].label) << "\n";
            }
          p.out << p.totalprint << std::endl;
          p.timetotal = 0.0;
          for (j = 0; j < p.totalprint; ++j)
            {
              if(write_data(p.out, p.simarr, p.timetotal, p.lightspeed))
                {
                  std::cerr << "Error: unable to save data to file '" << (s != "\0" ? s : argv[saveindex]) << "'.\n" << std::endl;
                }
              for (k = 0; k < p.printerval; ++k)
                {
                  collisions += temporal_evolve(p, timestep);
                }
            }
          p.out.close();
          std::cout << "The simulation is now over.\n\nIn case the user would like to continue simulating charges,"
                    << " there are two options:\n\n   (1) Load the configuration from a file\n\n (2) Input the"
                    << " information in the command line\n\nOtherwise, enter any other character to exit. " << std::flush;
          std::cin.clear();
          std::cin.sync();
          std::getline(std::cin, s);
          if(s.size() && s[0] == '1')
            {
              commandline_askfile(p);
            }
          else if(s.size() && s[0] == '2')
            {
              commandline_input( p);
            }
          else
            {
              break;
            }
        }
    }
  p.hist.rearrange();
  save_simple_config(LAST_CONFIG, p);
  std::cout << "\n\nThere were " << collisions << " collisions in total, in case the user was wondering...\n" << std::endl;
  return 0;
}

/* ***********************************************************************

   Wishlist/Aspectos que ficaram imperfeitos
   ou incompletos por questões de tempo ou
   dificuldade de implementação:

   - AutoCam: um sistema que analise as posições
   das diversas partículas e oriente
   a câmara devidamente, de modo a permitir
   uma experiência "cinematográfica" de
   visualização, em que o utilizador não
   necessita de mexer a câmara.

   - Movimento via rato: possibilitar o uso de "click and drag"
   para mexer a câmara e/ou as cargas
   (o que se torna bastante complicado
   de conseguir tornar intuitivo
   num espaço tridimensional...)

   - Movimento relativo: possibilidade de, quando seleccionada
   uma carga, a câmara ficar a segui-la.
   (Após tentativa de implementação,
   verifica-se ser difícil permitir
   alterar o ângulo da câmara devidamente,
   razão por que se omite, na versão
   presente do programa, esta funcionalidade.)

   *********************************************************************** */
