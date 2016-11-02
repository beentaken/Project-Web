#include "Sudoku.h" 
#include <cstring> 
#include <iostream>
#include <iomanip> 
#include <stddef.h> 
#define MAX_DUPLICATES3 
#include <cstdio>
#include <cstdlib> 
#if defined (__GNUC__)
#if defined (__clang__)typedeflongintoff64_t;
#endif
#include <unistd.h> 
#else 
#include "getopt.h" 
#endif 
constintMAX_BOARD_SIZE=10000;namespa
ce{boolSHOW_BOARDS=false;size_tSHOW_BOARDS_EVERY=1;boolCLEAR_SCREEN=false;
boolSHOW_MESSAGES=false;boolPAUSE=false;boolSHOW_SOLUTION=true;boolABORTED=false;
boolSHOW_STATS=false;}chargFile[FILENAME_MAX];char*gBoard=0;intBASESIZE=3;
Sudoku::SymbolTypeSYMBOL_TYPE=Sudoku::SYM_NUMBER;voidusage(void){std::cout<<
"Usage:sudoku[options]input_file\n";std::cout<<"\n";std::cout<<"Options:\n";std::cout<< "-a--
show_solution=Xshowfinalboard(0-OFF,1-ON,defaultisON).\n";std::cout <<"-b--
basesize=Xthesizeoftheboard(e.g.3,4,5,etc.Defaultis3)\n";std::cout <<"-c--
clear_screenclearthescreenbetweenboards.\n";std::cout <<"-e--
show_boards_every=XonlyshowboardsaftereveryXiterations.\n";std::cout<< "-h--
helpdisplaythisinformation.\n";std::cout<<"-m--
show_messagesshowallmessages(placing,removing,backtracking,etc.\n";std::cout<<"-p--
pausepausebetweenmoves(pressakeytocontinue).\n";std::cout<<"-s--
show_boardsshowboardsafterplacing/removing.\n";std::cout<<"-t--
symbol_type=Xthetypeofsymbolstouse,numbersorletters(0isnumbers,1isletters)\n";std::cout<<"-w--show_s
tats=Xshowstatisticsaftereachmove.\n";std::cout<<"\n";}voiddump_options(void){std::cout<<"show_solut
ion:"<<(SHOW_SOLUTION?"true":"false")<<std::endl;std::cout<<"basesize:"<<BASESIZE<<std::endl;std::co
ut<<"clear_screen:"<<(CLEAR_SCREEN?"true":"false")<<std::endl;std::cout<<"show_boards_every:"<<SHOW_
BOARDS_EVERY<<std::endl;std::cout<<"show_messages:"<<(SHOW_MESSAGES?"true":"false")<<std::endl;std::
cout<<"pause:"<<(PAUSE?"true":"false")<<std::endl;std::cout<<"show_boards:"<<(SHOW_BOARDS?"true":"fa
lse")<<std::endl;std::cout<<"symbol_type:"<<(SYMBOL_TYPE==Sudoku::SYM_NUMBER?"Numbers":"Letters")<<s
td::endl;std::cout<<"show_stats:"<<(SHOW_STATS?"true":"false")<<std::endl;std::cout<<"filename:"<<gF
ile<<std::endl;}voidparse_commandline(intargc,char**argv){intfail=0;intoption_index=0;staticstructop
tionlong_options[]={{"show_solution",required_argument,NULL,'a'},{"basesize",required_argument,NULL,
'b'},{"clear_screen",no_argument,NULL,'c'},{"show_boards_every",required_argument,NULL,'e'},{"help",
no_argument,NULL,'h'},{"show_messages",no_argument,NULL,'m'},{"pause",no_argument,NULL,'p'},{"show_b
oards",no_argument,NULL,'s'},{"symbol_type",required_argument,NULL,'t'},{"show_stats",no_argument,NU
LL,'w'},{NULL,0,NULL,0}};optind=1;for(;;){intopt=getopt_long(argc,argv,"-:a:b:ce:hmpst:w",long_optio
ns,&option_index);if(opt==-1)break;switch(opt){case0:break;case'a':if(std::atoi(optarg))SHOW_SOLUTIO
N=true;break;case'b':BASESIZE=std::atoi(optarg);break;case'c':CLEAR_SCREEN=true;break;case'e':if(std
::atoi(optarg))SHOW_BOARDS_EVERY=true;elseSHOW_BOARDS_EVERY=false;break;case'h':usage();std::exit(0)
;case'm':SHOW_MESSAGES=true;break;case'p':PAUSE=true;break;case's':SHOW_BOARDS=true;break;case't':if
(std::atoi(optarg))SYMBOL_TYPE=Sudoku::SYM_LETTER;elseSYMBOL_TYPE=Sudoku::SYM_NUMBER;break;case'w':S
HOW_STATS=true;break;case'?':std::cout<<"Unknownoption:"<<optopt<<std::endl;fail=1;break;case':':std
::cout<<"Missingoptionfor"<<optopt<<std::endl;fail=1;break;case1:std::strcpy(gFile,optarg);break;}}i
f(fail){usage();std::exit(-1);}}voidPrintStats(constSudoku&board){Sudoku::SudokuStatsstats=board.Get
Stats();std::cout<<"Basesize:"<<stats.basesize<<std::endl;std::cout<<"Moves:"<<stats.moves<<std::end
l;std::cout<<"Placed:"<<stats.placed<<std::endl;std::cout<<"Backtracks:"<<stats.backtracks<<std::end
l;std::cout<<"Filename:"<<gFile<<std::endl;}voidDumpBoard(constchar*board,intbasesize,size_tmoves){i
ntblocksize=basesize*basesize;std::cout<<"Moves:"<<moves<<std::endl;//twospacesforeachsymbolintlen=(
blocksize*2)+(basesize*2)+1;//printtophorizontallinewith-and+intheappropriatelocationsfor(intk=0;k<l
en;k++){if((k==0)||!(k%(2*basesize+2)))std::cout<<"+";elsestd::cout<<"-";}std::cout<<std::endl;intco
unt=0;for(inti=0;i<blocksize*blocksize-1;i+=blocksize){std::cout<<"|";for(intj=0;j<basesize;j++){for
(intk=0;k<basesize;k++){std::cout<<std::setw(2)<<board[i+j*basesize+k+0];}std::cout<<"|";//if(j&&((j
==2)||(j==5)||(j==8)))}std::cout<<std::endl;count++;//printhorizontallineswith-and+intheappropriatel
ocations//aftereach'set'ofsymbolsif(!(count%basesize)){for(intk=0;k<len;k++){if((k==0)||!(k%(2*bases
ize+2)))std::cout<<"+";elsestd::cout<<"-";}std::cout<<std::endl;}}}voidPause(void){if(ABORTED)return
;if(std::fgetc(stdin)=='x'){std::cout<<"Userabort.\n";ABORTED=true;//std::exit(1);}}structRowCol{int
row;intcol;};intindex_to_row(intindex,intbasesize){returnindex/(basesize*basesize);}intindex_to_col(
intindex,intbasesize){returnindex%(basesize*basesize);}RowColIndexToRowCol(intindex,intbasesize){Row
Colrc;rc.row=index_to_row(index,basesize);rc.col=index_to_col(index,basesize);returnrc;}voidDisplayD
uplicates(int*duplicates,intbasesize){//std::cout<<duplicates[0]<<","<<duplicates[1]<<","<<duplicate
s[2]<<std::endl;if(duplicates[0]==-1&&duplicates[1]==-1&&duplicates[2]==-1)return;RowColrc;std::cout
<<"Duplicates:";intcount=0;if(duplicates[0]!=-1){rc=IndexToRowCol(duplicates[0],basesize);std::cout<
<"#"<<++count<<"atcell"<<duplicates[0]<<"("<<rc.row+1<<","<<rc.col+1<<")";}if(duplicates[1]!=-1){rc=
IndexToRowCol(duplicates[1],basesize);std::cout<<"#"<<++count<<"atcell"<<duplicates[1]<<"("<<rc.row+
1<<","<<rc.col+1<<")";}if(duplicates[2]!=-1){rc=IndexToRowCol(duplicates[2],basesize);std::cout<<"#"
<<++count<<"atcell"<<duplicates[2]<<"("<<rc.row+1<<","<<rc.col+1<<")";}}boolCallback(constSudoku&sud
oku,constchar*board,Sudoku::MessageTypemessage,size_tmove,intbasesize,intindex,charsymbol,int*duplic
ates){if(CLEAR_SCREEN&&message!=Sudoku::MSG_ABORT_CHECK)#ifdefined(__GNUC__)if(std::system("clear"))
#elseif(std::system("cls"))#endifstd::exit(0);switch(message){caseSudoku::MSG_STARTING:if(SHOW_MESSA
GES)std::cout<<"Starting...\n";DumpBoard(board,basesize,move);if(SHOW_STATS)PrintStats(sudoku);std::
cout<<std::endl;break;caseSudoku::MSG_PLACING:if(SHOW_MESSAGES){RowColrc=IndexToRowCol(index,basesiz
e);std::cout<<"Move:"<<move<<",Placing"<<symbol<<"at"<<rc.row+1<<","<<rc.col+1<<"(cell:"<<index+1<<"
).";std::cout<<"";DisplayDuplicates(duplicates,basesize);std::cout<<std::endl;}if(SHOW_BOARDS){if(!(
move%SHOW_BOARDS_EVERY)){DumpBoard(board,basesize,move);if(SHOW_STATS)PrintStats(sudoku);std::cout<<
std::endl;}}break;caseSudoku::MSG_REMOVING:if(SHOW_MESSAGES){RowColrc=IndexToRowCol(index,basesize);
std::cout<<"Move:"<<move<<",Removing"<<symbol<<"from"<<rc.row+1<<","<<rc.col+1<<"(cell:"<<index+1<<"
).\n";}if(SHOW_BOARDS){if(!(move%SHOW_BOARDS_EVERY)){DumpBoard(board,basesize,move);if(SHOW_STATS)Pr
intStats(sudoku);std::cout<<std::endl;}}break;caseSudoku::MSG_FINISHED_OK:std::cout<<"Solutionfoundi
n"<<move<<"moves.\n";if(SHOW_SOLUTION)DumpBoard(board,basesize,move);break;caseSudoku::MSG_FINISHED_
FAIL:std::cout<<"Nosolutionfoundafter"<<move<<"moves\n";break;caseSudoku::MSG_ABORT_CHECK:if(ABORTED
)returntrue;break;default:std::cout<<"Unknownmessage.\n";}if(PAUSE)Pause();returnfalse;}voidstripnl(
char*line){size_tlen=std::strlen(line);line[len-1]=0;}voidacquire_board(constchar*filename){std::FIL
E*fp=std::fopen(filename,"rt");if(!fp){std::cout<<"Failedtoopen"<<filename<<"forinput.\n";std::exit(
-1);}//TODO:Allocatedynamicallycharbuffer[MAX_BOARD_SIZE];while(!std::feof(fp)){if(std::fgets(buffer
,MAX_BOARD_SIZE,fp)==NULL)break;if(buffer[0]=='#')continue;stripnl(buffer);gBoard=newchar[strlen(buf
fer)+1];std::strcpy(gBoard,buffer);break;}std::fclose(fp);}typedefvoid(*TestFn)(void);voidExecuteTes
t(TestFnfn,intmaxwait=1000,intsafewait=1000){if(maxwait+safewait>0)fn();}voidRunTest(){acquire_board
(gFile);Sudokuboard(BASESIZE,SYMBOL_TYPE,Callback);board.SetupBoard(gBoard,static_cast<int>(std::str
len(gBoard)));std::cout<<"\n====================";std::cout<<"Board:["<<BASESIZE<<"x"<<BASESIZE<<"]"
<<gFile;std::cout<<"====================\n";board.Solve();PrintStats(board);std::cout<<std::endl;del
ete[]gBoard;}intmain(intargc,char**argv){parse_commandline(argc,argv);if(!std::strlen(gFile)){usage(
);std::exit(1);}structTimedTest{void(*test)(void);intmaxwait;intsafewait;};intmax=500;intsafe=500;Ti
medTestTest={RunTest,max,safe};ExecuteTest(Test.test,Test.maxwait,Test.safewait);return0;}Sudoku::Su
doku(intbasesize,SymbolTypestype,SUDOKU_CALLBACKcallback){
board_=newchar[(basesize*basesize)*(basesize*basesize)];    stats_=newSudokuStats;
duplicates_=newint[MAX_DUPLICATES]; for(inti=0;i<MAX_DUPLICATES;++i)        duplicates_[i]=-1;
type_=stype;    exit_=false;    stats_->basesize=basesize;  Callback_=callback;}Sudoku::~Sudoku(){de
lete[](board_);delete(stats_);delete[](duplicates_);}constchar*Sudoku::GetBoard()const{returnboard_;
}Sudoku::SudokuStatsSudoku::GetStats()const{return*stats_;}voidSudoku::SetupBoard(constchar*values,i
ntsize){    for(inti=0;i<size;++i){     if(values[i]=='.'){         board_[i]=EMPTY_CHAR; }else{
board_[i]=values[i];        }   }}voidSudoku::Solve(){
intgridsize=(stats_->basesize*stats_->basesize);    intlast_index=(gridsize*gridsize)-1;
charstart;if(type_==SYM_LETTER) start='A';if(type_==SYM_NUMBER) start='1';
Callback_(*this,board_,MSG_STARTING,0,stats_->basesize,0,start,duplicates_);stats_->moves++;
boolresult=place_value(0);stats_->moves--;  if(result==true){       Callback_(*this,board_,MSG_FINIS
HED_OK,stats_->moves,stats_->basesize,last_index,board_[last_index],duplicates_);   }else{      Call
back_(*this,board_,MSG_FINISHED_FAIL,stats_->moves,stats_->basesize,last_index,board_[last_index],du
plicates_); }   return;}boolSudoku::place_value(intindex){
intgridsize=(stats_->basesize*stats_->basesize);    intlast_index=(gridsize*gridsize)-1;
boolconflict;   charstart;  chartemp;   boolret;    while(board_[index]!=EMPTY_CHAR){
if(index==last_index)           returntrue;     index++;    }   if(Callback_(*this,board_,MSG_ABORT_
CHECK,stats_->moves,stats_->basesize,index,board_[index],duplicates_)==true){       exit_=true;
returnfalse;    }stats_->placed++;  if(type_==SYM_LETTER)       start='A';  if(type_==SYM_NUMBER)
start='1';charlastval=(char)(start+gridsize-1); board_[index]=start;    conflict=Conflict(index);
Callback_(*this,board_,MSG_PLACING,stats_->moves,stats_->basesize,index,board_[index],duplicates_);
while(board_[index]<lastval){       if(conflict==false){            stats_->moves++;
if(index==last_index)               returntrue;         ret=place_value(index+1); if(exit_==true)
returnfalse;            if(ret==true)               returntrue;     } temp=board_[index];
board_[index]=EMPTY_CHAR;   stats_->placed--;
Callback_(*this,board_,MSG_REMOVING,stats_->moves,stats_->basesize,index,temp,duplicates_);
stats_->placed++;       board_[index]=temp;     board_[index]++;            stats_->moves++;
conflict=Conflict(index);
Callback_(*this,board_,MSG_PLACING,stats_->moves,stats_->basesize,index,board_[index],duplicates_);
}   if(board_[index]==lastval){     if(conflict==false){            if(index==last_index){
returntrue;         }           stats_->moves++;            ret=place_value(index+1);
if(exit_==true)             returnfalse;            if(ret==true)               returntrue;     }
stats_->placed--;       temp=board_[index];     board_[index]=EMPTY_CHAR;
Callback_(*this,board_,MSG_REMOVING,stats_->moves,stats_->basesize,index,temp,duplicates_);
stats_->backtracks++;       returnfalse;    }   returntrue;}boolSudoku::Conflict(intindex){
intgridsize=(stats_->basesize*stats_->basesize);    intlast_index=(gridsize*gridsize)-1;
intcheck_index=index;   intright_values=index%gridsize;;
intleft_values=gridsize-((index%gridsize)+1);   intconf1=-1;intconf2=-1;intconf3=-1;
boolconflict=false; while(check_index-gridsize>=0){     check_index=check_index-gridsize;
if(board_[check_index]==board_[index]){         conf1=check_index;          conflict=true;      } }
check_index=index;  while(check_index+gridsize<=last_index){ check_index=check_index+gridsize;
if(board_[check_index]==board_[index]){ conf1=check_index;          conflict=true;      }   }
check_index=index; for(inti=1;i<=right_values;++i){            if(board_[index-i]==board_[index]){
conf2=index-i;          conflict=true;          break;      }   }   for(inti=1;i<=left_values;++i){
if(board_[index+i]==board_[index]){         conf2=index+i;          conflict=true;          break; }
}   intsection_size=gridsize*stats_->basesize;check_index=index;
//////////////////////////////////////////////////////////////////////////////////////////
//CheckNeighborhoodAbove//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
while(!(check_index%section_size<gridsize&&check_index%stats_->basesize==0)){
if(board_[check_index]==board_[index]&&index!=check_index){         conf3=check_index;
conflict=true;          break;      }       if(check_index%stats_->basesize==0)
check_index-=((stats_->basesize-1)*stats_->basesize)+1;     else            check_index--;  }
if(board_[check_index]==board_[index]&&index!=check_index){     conf3=check_index; conflict=true;  }
//////////////////////////////////////////////////////////////////////////////////////////  ////////
//////////////////////////////////////////////////////////////////////////////////check_index=index;
while(!(check_index%stats_->basesize==(stats_->basesize-1)&&check_index%section_size>(stats_->basesi
ze-1)*gridsize)){       if(board_[check_index]==board_[index]&&index!=check_index){
conf3=check_index;          conflict=true;          break;      }
if(check_index%stats_->basesize==(stats_->basesize-1))
check_index+=((stats_->basesize-1)*stats_->basesize)+1;     else            check_index++;  }
if(board_[check_index]==board_[index]&&index!=check_index){     conf3=check_index; conflict=true;  }
//Noduplicatesatstartfor(inti=0;i<MAX_DUPLICATES;++i)
duplicates_[i]=9999;if(conf1==conf2&&conf1==conf3){ conf3=-1;   conf2=-1;}if(conf1==conf3)
conf3=-1;if(conf1==conf2)   conf2=-1;if(conf2==conf3)   conf3=-1;Insert_Conflict(conf1);Insert_Confl
ict(conf2);Insert_Conflict(conf3);for(inti=0;i<MAX_DUPLICATES;++i){ if(duplicates_[i]==9999)
duplicates_[i]=-1;} if(conflict==true)      returntrue; returnfalse;}/*!Insertselementsinasortedmann
er/paramelementbeinginserted*/voidSudoku::Insert_Conflict(intindex){    inti=2;
while((i>0)&&(index<duplicates_[i-1])){ duplicates_[i]=duplicates_[i-1];    i--;}
duplicates_[i]=index;}
