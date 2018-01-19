/* Ad-hoc programming editor for DOSBox -- (C) 2011-03-08 Joel Yliluoma */
#include "langdefs.hh"

#ifdef __DJGPP__
# include <dos.h>
# include <dpmi.h>
# include <go32.h>
#endif
#ifdef __BORLANDC__
# include <dos.h>
#endif

//#include <string.h>

#include "vga.hh"

volatile unsigned long MarioTimer = 0;

static const char Mario[] =
//123456789ABCDEF                0123456789ABCDEF
//               0123456789ABCDEF
/*
"      #####                           #####     "
"     #########         #####         #########  "
"     %%%''%'          #########      %%%''%'    "
"    %'%'''%'''        %%%''%'       %'%'''%'''  "
"    %'%%'''%'''      %'%'''%'''     %'%%'''%''' "
"    %%''''%%%%       %'%%'''%''     %%''''%%%%  "
"      '''''''        %%''''%%%%       '''''''   "
"     %%#%%%            '''''''     %%%%##%%     "
"    %%%%##%%          %%%%#% '   ''%%%%###%%%'''"
"    %%%##'##'        '%%%%%%'''  ''' %%#'###%%''"
"    %%%%#####       ''#%%%%%''   ''  #######  % "
"    #%%'''###       %%#######       #########%% "
"     #%''###        %########      ##########%% "
"      ###%%%       %%### ###      %%###   ###%% "
"      %%%%%%%      %    %%%       %%%           "
"      %%%%              %%%%       %%%          "*/

//123456789ABCDEF
//               0123456789ABCDEF
"                      #####     "
"      ######         #'''''###  "
"     #''''''##      #'''''''''# "
"    #'''''''''#     ###'.#.###  "
"    ###..#.###     #..##.#....# "
"   #..##.#....#    #..##..#...# "
"   #..##..#...#     ##...#####  "
"    ##...#####      ###.....#   "
"     ##.....#     ##'''##''###  "
"    #''##''#     #..''''##''#'# "
"   #''''##''#    #..'''######'.#"
"   #''''#####     #..####.##.#.#"
"    #...##.##     .#########''# "
"    #..'''###     #''######'''# "
"     #'''''#      #'''#  #'''#  "
"      #####        ###    ###   ";

static const char Mario21[] =
"                       #####    "
"       #####         ##'''Y'#   "
"     ##'''Y'#       #''++YYY#   "
"    #''++YYY#      #+'++######  "
"   #+'++######    #+++######### "
"  #+++#########   #.###:#:#:    "
"  #.###:#:#:     #.#.#:.#.#.##  "
" #.#.#:.#.#.##   #:#.##.......# "
" #:#.##.......#  ##:.#..#:::::# "
" ##:.#..#:::::#   ##::.#######  "
"  ##::.#######     ##::::####   "
"   ###:::####      #+#####%     "
"    #+####%        #++'#,11%    "
"   #++',,11%      ##...#..1.%## "
"   ####,..1.%    ###..#,..1.#Y##"
"   #...#..1.%    ###..#,,,1%####"
"   #..#,,,11%    ##%##,,,,%#### "
"   #..#,,%,%     ##Y# %%%% #### "
"    #######       ##        ##  "
"    ####Y#Y#                    "
"    ########                    ";
static const char Mario32[] =
//123456789ABCDEF0123456789ABCDEFGH0123456789ABCDEF0123456789ABCDEFGH0123456789ABCDEF0123456789ABCDEFGH""""""""
"                                                                                                      "
"                                                                                                      "
"                                                                                                      "
"           ##   ##                           ##   ##                           ##   ##                "
"           #.######                          #.######                          #.######               "
"           #..#,,,,#                         #..#,,,,#                         #..#,,,,#              "
"          #,..,,,,,#                        #,..,,,,,#                        #,..,,,,,#              "
"         #,,.,,,,,,,#                      #,,.,,,,,,,#                      #,,.,,,,,,,#             "
"         #,,,,,,,,,,#                      #,,,,,,,,,,#                      #,,,,,,,,,,#             "
"        #,,,,,,,,,,,#                     #,,,,,,,,,,,#                     #,,,,,,,,,,,#             "
"        #,,,,,,####,#                     #,,,,,,####,#                     #,,,,,,####,#             "
"        #,,,,,#..#.#                      #,,,,,#..#.#                      #,,,,,#..#.#              "
"        #,,,,,...#.###                    #,,,,,...#.###                    #,,,,,...#.###            "
"        #,,,,,........#######             #,,,,,........#######             #,,,,,........#######     "
"         #,,,,..#.....##....##             #,,,,..#.....##....##             #,,,,..#.....##....##    "
"         #,,,,.###...##.......#            #,,,,.###...##.......#            #,,,,.###...##.......#   "
"          ##,,...#####.######..##           ##,,...#####.######..##           ##,,...#####.######..## "
"           ####....#..#,,,,,,#...#           ####....#..#,,,,,,#...#           ####....#..#,,,,,,#...#"
"          #,#,,####.###,,,,,,###..          #,#,,####.###,,,,,,###..          #,#,,####.###,,,,,,###.."
"          ##,,,,,####,,#,,,,#,,###          ##,,,,,####,,#,,,,#,,###          ##,,,,,####,,#,,,,#,,###"
"         #,#,,,,,,,##,,,####,,,,,#         #,#,,,,,,,##,,,####,,,,,#         #,#,,,,,,,##,,,####,,,,,#"
"         #,,#,,,,,#..#,#,,,,#,,,##         #,,#,,,,,#..#,#,,,,#,,,##         #,,#,,,,,#..#,#,,,,#,,,##"
"       ###,,,##,,#....#,,,,,,#,##     ######,,,##,,#....#,,,,,,#,##  #########,,,##,,#....#,,,,,,#,## "
"     ##..#,,,,,###....#,,,,,,,#,#   ###,,..#,,,,,###....#,,,,,,,#,# ##,..,,...#,,,,###....#,,,,,,,#,# "
"    #.,..#,,,,,,,##..##,,,,,,#,#   ##..,,..#,,,,,,,##..##,,,,,,#,#  #,,..,,...#,,,,,,##..##,,,,,,#,#  "
"   #...,.#,,,,,,,..##,,#,,,,#,,#   #,,..,,#,,,,,,,,..##,,#,,,,#,,#  #,,..,,..#,,,,,,,,.##.##,,,,#,,#  "
"  #,,..,# #,,,,,,,..##,,####,,#    #,,..,,#,,,,,,,,,..,#,,####,,#   ##,..,,##..,,,,,,,,...#,####,,#   "
"  #,,..#  #,,,,,,,,,# ##,,,,##     ##,,###,,,,,,,#,,,,,.##,,,,##     #######..,,,,,###,...##,,,,##    "
"  #,,,#    #,,,,,,##   ######       ###   #.,,,,# #,,,..#######            #..,,###   #..########     "
"  ##,,#    #,,,,,#                        #..,,#   #,..#                   #..##       ###            "
"   ###     #......#                        #....#  #..#                     ##                        "
"           ########                         ####    ##                                                "
;

static unsigned OverlayMarioByte(
    unsigned pose,
    register unsigned y, register unsigned absx,
    register unsigned char OriginalByte,
    signed int MarioOffset)
{
    const char* data       = nullptr;
    int max_x              = 16;
    unsigned NumMarioPoses = 2;

    if(VidCellHeight <= 16)
    {
        data = (Mario + 0 + pose * unsigned(16+0) + y * unsigned(0*(NumMarioPoses+1)+16*NumMarioPoses));
    }
    else if(VidCellHeight <= 20)
    {
        data = (Mario21 + 0 + pose * unsigned(16+0) + y * unsigned(0*(NumMarioPoses+1)+16*NumMarioPoses));
    }
    else
    {
        NumMarioPoses = 3;
        data = (Mario32 + 0 + pose * unsigned(34+0) + y * unsigned(0*(NumMarioPoses+1)+34*NumMarioPoses));
        max_x = 34;
    }

    static const unsigned char dither4x4[16] =
        {0,12,3,15, 8,4,11,7, 2,14,1,13, 10,6,9,5};
    const unsigned char* dithline = dither4x4 + (((y+MarioTimer) & 3u) * 4u);

    unsigned char byte = 0x00;
    int xoffs = MarioOffset;
    for(register unsigned x=0; x<8; ++x, ++xoffs)
    {
        unsigned char dithval = dithline[ (x+absx)&3 ];
        register unsigned char bit = 1;
        char c = (unsigned(xoffs) < unsigned(max_x)) ? (data[xoffs] / 3) : ('?' / 3);
        switch(c)
        {
            case '.' / 3: // 15
                bit = 0;            // 0%
                break;
            case 'Y' / 3: // 29
                bit = dithval < 2;  // 12.5%
                break;
            case ':' / 3: // 19
                bit = dithval < 4;  // 25%
                break;
            case ',' / 3: // 14
            case '\'' / 3: // 13
                bit = dithval >> 3; // 50%
                break;
            case '%' / 3: // 12
                bit = dithval >= 4; // 75%
                break;
            case '#' / 3: // 11
                break;              // 100%
            default:
            case '?' / 3: // 21
            case ' ' / 3: // 10
                // transparent, no change
                byte |= OriginalByte & (0x80 >> x);
                continue;
        }
        if(bit)
            byte |= 1u << (7-x);
    }
    return byte;
}

void MarioTranslate(
    unsigned long*  model,
    unsigned short* target,
    unsigned width)
{
    const unsigned room_left   = 240;
    const unsigned room_right  = 8;
    const unsigned room_wide   = width * (FatMode ? 16 : 8);
    const unsigned xspanlength = room_wide + room_left + room_right;
    //const unsigned twospans = xspanlength * 2u;
    unsigned long mt = MarioTimer / 2;
    const unsigned poses = (VidCellHeight >= 29) ? 4u : 2u;

    const unsigned MarioStepInterval = VidCellHeight >= 29 ? 5 : 7;
    //if(VidCellHeight >= 29) mt *= 2;

    unsigned marioframe = (mt / MarioStepInterval) % poses;
    if(marioframe == 3) marioframe = 1;

    /*
    unsigned mariopos = timer % twospans;
    int mariox = mariopos < xspanlength
        ? (int)mariopos - (int)room_left
        : (int)room_wide + (int)room_right - (int)(mariopos - xspanlength);
    */
    int mariox = (mt % xspanlength) - room_left;

    unsigned char RevisedFontData[ 6 * 32 ];

    const unsigned MarioColor = 0x0800;

    const unsigned base = FatMode ? 0x80 : 0xC0;

    const unsigned short chartable[6] =
        { (unsigned short)(base + 0x0 + MarioColor),
          (unsigned short)(base + 0x3 + MarioColor),
          (unsigned short)(base + 0x4 + MarioColor),
          (unsigned short)(base + 0x8 + MarioColor),
          (unsigned short)(base + 0x9 + MarioColor),
          (unsigned short)(base + 0xD + MarioColor)
        };

    unsigned fontdatasize  = 0;
    unsigned numchars      = 0;
    unsigned spritewide = 16;
    if(VidCellHeight >= 29) spritewide = 34;
    for(int basex = mariox - (mariox % 8)
      ; basex < int(mariox + spritewide)
     && basex < int(room_wide)
      ; basex += 8)
    {
        if(basex < 0) continue;

        int offset = basex - mariox;

        unsigned long word = model[basex >> 3];
        unsigned char ch = word;

        if(ch == 0xDC || (ch >= 0xB0 && ch <= 0xB2))
            ch = 0x20;

        const unsigned char* SourceFontPtr = VgaFont + (ch * VidCellHeight);

        for(unsigned y=0; y<VidCellHeight; ++y)
        {
            RevisedFontData[fontdatasize++] =
                OverlayMarioByte(marioframe,y,mariox, SourceFontPtr[y], offset);
        }
        if(word & 0x80000000ul)
            model[basex >> 3] = (word & 0xBFFF8000ul) | chartable[numchars++];
        else
            model[basex >> 3] = (word & 0x3FFF0000ul) | chartable[numchars++];
    }

    for(unsigned p=room_wide/8; p-- > 0u; )
    {
        unsigned long v = *model;
        *target = v;
        target[(DOSBOX_HICOLOR_OFFSET/2)] = v >> 16;
        ++target;
        ++model;
    }
    //memcpy(target, model, room_wide/4);
#ifdef __BORLANDC__
    if(numchars > 0)
    {
        // Update VGA font:
        _asm {
            push es
            push bp
            push di
             mov ax, 0x1100
             mov bh, 0
             mov bl, VidCellHeight
             mov di, base
             mov si, bx
             xchg bl,bh
             push ss
             pop es
             lea bp, offset RevisedFontData
             lea dx, [di+0x0]; mov cx, 1; int 0x10
              add bp, si
              shl si, 1
             lea dx, [di+0x3]; mov cx, 2; int 0x10
              add bp, si
             lea dx, [di+0x8]; mov cx, 2; int 0x10
              add bp, si
             lea dx, [di+0xD]; mov cx, 1; int 0x10
            pop di
            pop bp
            pop es
        }
    }
#endif
#ifdef __DJGPP__
    if(numchars > 0)
    {
        VgaEnableFontAccess();
        VgaSetFont(VidCellHeight, 1, base+0x0, RevisedFontData+0);
        VgaSetFont(VidCellHeight, 2, base+0x3, RevisedFontData+VidCellHeight);
        VgaSetFont(VidCellHeight, 2, base+0x8, RevisedFontData+VidCellHeight*3);
        VgaSetFont(VidCellHeight, 1, base+0xD, RevisedFontData+VidCellHeight*5);
        VgaDisableFontAccess();
    }
#endif
}

#ifdef __BORLANDC__
static unsigned short rate=120U, Clock=0u, Counter=0x1234DCUL/rate;
static void (interrupt *OldI08)();
static void interrupt MarioI08()
{
    MarioTimer += 1;
    _asm {
        mov ax, Counter
        add Clock, ax
        jnc P1
    }
    OldI08();
    goto P2;
P1: _asm { mov al, 0x20; out 0x20, al }
P2:;
}
#endif
#ifdef __DJGPP__
static unsigned rate=60U, Clock=0u, Counter=0x1234DCUL/rate;
static _go32_dpmi_seginfo OldI08_rm, NewI08_rm;
static _go32_dpmi_seginfo OldI08_pm, NewI08_pm;
static _go32_dpmi_registers I08regs;

static bool Count()
{
    MarioTimer += 2;
    Clock += Counter;
    if(Clock & 0x10000)
    {
        Clock &= 0xFFFF;
        return true;
    }
    return false;
}


static void MarioI08_rm()
{
    if(Count())
    {
        I08regs.x.cs = OldI08_rm.rm_segment;
        I08regs.x.ip = OldI08_rm.rm_offset;
        I08regs.x.ss = I08regs.x.sp = 0;
        //__dpmi_simulate_real_mode_procedure_iret(&I08regs);
        _go32_dpmi_simulate_fcall_iret(&I08regs);
        //outportb(0x20,0x20);
    }
    else
    {
        outportb(0x20,0x20);
    }
}
static void MarioI08_pm()
{
    if(Count())
    {
    #if 0
        _farpokel(_dos_ds, 0x46C, _farpeekl(_dos_ds, 0x46C)+1);
    #else
        I08regs.x.cs = OldI08_rm.rm_segment;
        I08regs.x.ip = OldI08_rm.rm_offset;
        I08regs.x.ss = I08regs.x.sp = 0;
        __dpmi_simulate_real_mode_procedure_iret(&I08regs);
    #endif
    }
    outportb(0x20,0x20);
}
#endif

void FixMarioTimer()
{
    disable();
#ifdef __BORLANDC__
    _asm { mov al, 0x34;    out 0x43, al
           mov ax, Counter; out 0x40, al
           mov al, ah;      out 0x40, al }
#endif
#ifdef __DJGPP__
    outportb(0x43,0x34);
    outportb(0x40, Counter);
    outportb(0x40, Counter>>8);
#endif
    enable();
}

void InstallMario()
{
#ifdef __BORLANDC__
    disable();
    OldI08 = (void(interrupt*)()) *(void **)MK_FP(0, 8*4);
    *(void **)MK_FP(0, 8*4) = (void *)MarioI08;
#endif
#ifdef __DJGPP__
    _go32_dpmi_get_real_mode_interrupt_vector(8, &OldI08_rm);
    NewI08_rm.pm_offset = (unsigned long)MarioI08_rm;
    NewI08_rm.pm_selector = _go32_my_cs();
    _go32_dpmi_allocate_real_mode_callback_iret(&NewI08_rm, &I08regs);
    _go32_dpmi_set_real_mode_interrupt_vector(8, &NewI08_rm);

    _go32_dpmi_get_protected_mode_interrupt_vector(8, &OldI08_pm);
    NewI08_pm.pm_offset   = reinterpret_cast<unsigned long>(MarioI08_pm);
    NewI08_pm.pm_selector = _go32_my_cs();
    _go32_dpmi_allocate_iret_wrapper(&NewI08_pm);
    _go32_dpmi_set_protected_mode_interrupt_vector(8, &NewI08_pm);
#endif
    FixMarioTimer();
}

void DeInstallMario()
{
#ifdef __BORLANDC__
    disable();
    *(void **)MK_FP(0, 8*4) = (void *)OldI08;
    _asm { mov al, 0x34;    out 0x43, al
           xor al, al;      out 0x40, al
           /*********/      out 0x40, al }
    enable();
#endif
#ifdef __DJGPP__
    _go32_dpmi_set_real_mode_interrupt_vector(8, &OldI08_rm);
    _go32_dpmi_set_protected_mode_interrupt_vector(8, &OldI08_pm);
    _go32_dpmi_free_iret_wrapper(&NewI08_pm);
    disable();
    outportb(0x43,0x34);
    outportb(0x40,0);
    outportb(0x40,0);
    enable();
#endif
}