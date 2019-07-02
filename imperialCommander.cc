//NIE Y2230091W CORREAL BOTERO,EDUARDO

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

typedef struct {
     string type;  // TIE Fighter, X-Wing, ...
     int velocity;
     int attack;    // attack power
     int shield;    // current shield status. if <=0, the fighter is considered destroyed
     int cost;      // cost in credits
} Fighter;                                        


typedef struct {
    bool side;       // IMPERIAL or REBEL
    int maxCapacity; // maximum capacity of fighters allowed in the ship
    int credits;      // credits remaining to buy fighters, make repairs or improvements
    int wins;         // enemy fighters destroyed
    int losses;        // fighters lost
    vector<Fighter> fighters;  // fighters inside the ship
} Ship;


// Fighters table 

const int MAXFIGHTERS=8;

// abbreviations (to use in option "Add fighter")
const string FIGHTERABR[]= { "tf", "tb", "ti", "ta", 
                             "xw", "yw", "aw", "bw"
};

const Fighter FIGHTERTABLE[] = {
 { "TIE-Fighter",      150,  75, 30,  45 },
 { "TIE-Bomber",        80, 150, 45,  75 },
 { "TIE-Interceptor",  180,  65, 30,  55 },
 { "TIE-Advanced",     160,  80, 90,  95 },
 { "X-Wing",           175,  90, 75,  65 },
 { "Y-Wing",            90, 150, 90,  90 },
 { "A-Wing",           200,  60, 50,  45 },
 { "B-Wing",           120, 200, 90, 100 }
};


// ship constants
const bool IMPERIAL=false;
const bool REBEL=true;

const int IMPSHIPCAPACITY=30;
const int REBSHIPCAPACITY=35;

const string initialImperialShipDotation="10tf/ 5 tb, 5 ti, 5;ta";
// 10 TIE-Fighters, 5 TIE-Bombers, 5 TIE-Interceptors, 5 TIE-Advanced, 0 X-Wing, ....

const string initialRebelShipDotation="10xw,5yw, 8aw, 5bw";
// 0 TIE-Fighters ..., 10 X-Wing, 5 Y-Wing, 8 A-Wing, 5 B-Wing

// ship initial credits
const int CREDITS_INITIAL=2000;

// error numbers
const int UNKNOWN_OPTION=1;
const int WRONG_FIGHTER_TYPE=2;
const int CAPACITY_EXCEEDED=3;
const int WRONG_NUMBER=4;
const int NO_FUNDS=5;
const int NO_FIGHTERS=6;

//---------------------------------------------------
void error(int n)
{
  cout << "ERROR (" << n << "): " ;
  
  switch (n) 
  {
    case UNKNOWN_OPTION:
      cout << "unknown option" << endl;
      break;
    case WRONG_FIGHTER_TYPE:
      cout << "wrong fighter type" << endl;
      break;
    case CAPACITY_EXCEEDED:
      cout << "ship capactity exceeded" << endl;
      break;
    case WRONG_NUMBER:
      cout << "wrong number" << endl;
      break;
    case NO_FUNDS:
      cout << "not enough credits" << endl;
      break;
    case NO_FIGHTERS:
      cout << "not enough fighters" << endl;
      break;
  }
}

//---------------------------------------------------
int getRandomNumber(int max)
{
   // Genera un número aleatorio entre 0 y maximo-1
   //
   int na;

   na = (int)(((double)max)*rand()/(RAND_MAX+1.0));
   return na;
}
//---------------------------------------------------
void FighterType(Ship &ship,const int &NumberFighter,const string &abreviature, bool &addedFighter)
{
  bool CorrectAbreviature=false; // utilizado para saber si el tipo de caza es correcto.

  if(ship.side==IMPERIAL)
  {
    for(int i=0; i<4;i++) 
    {
      if(abreviature==FIGHTERABR[i]) //para saber que tipo de caza es
      {
        CorrectAbreviature=true;
        if(NumberFighter*FIGHTERTABLE[i].cost>ship.credits)
        {
          error(NO_FUNDS);
          addedFighter=false;
        }
        else
        {
          if(NumberFighter+ship.fighters.size()>ship.maxCapacity)
          {
            error(CAPACITY_EXCEEDED);
            addedFighter=false;
          }
          else
          {
            for(int j=0;j<NumberFighter;j++) // si las condiciones anteriores no se cumplen, se agregan el número de cazas a la nave imperial.
            {
              ship.fighters.push_back(FIGHTERTABLE[i]);
              ship.credits -= FIGHTERTABLE[i].cost;  // se descuenta el coste de los cazas agregados.
            }
          }
        } 
      }
    }
    if(CorrectAbreviature==false)
    {
      error(WRONG_FIGHTER_TYPE);
      addedFighter=false;
    }
  }
  else
  {
   for(int i=4; i<MAXFIGHTERS;i++) 
   {
     if(abreviature==FIGHTERABR[i]) //para saber que tipo de caza es
      {
        for(int j=0;j<NumberFighter;j++) 
        {
          ship.fighters.push_back(FIGHTERTABLE[i]);
          ship.credits -= FIGHTERTABLE[i].cost;  // se descuenta el coste de los cazas agregados.
        }
      }
    }
  }
}

//---------------------------------------------------
bool InitializeAndAdd(const string &OptionFighter, Ship &ship)
{
  int NumberFighter; // guarda el número de cazas que se quieren agregar de cada tipo.
  bool addedFighter=true; //variable usada para saber si se han agregado todos los caas satisfactoriamente.

  for(int i=0;i<OptionFighter.length() && addedFighter;i++)
  {
    string abreviature;
    string aux;// almacena los valores de los números y letra para luego pasarlos a la variables que contienen el número y el tipo.
    if((isdigit(OptionFighter[i]) ))
    { 
      aux=OptionFighter[i];
      NumberFighter=atoi(aux.c_str());
      if((OptionFighter[i+1]>='0' && OptionFighter[i+1]<='9'))
      {
        aux=OptionFighter[i+1];
        NumberFighter = NumberFighter*10 + atoi(aux.c_str());
        i++;
      }
    }
    while(isalpha(OptionFighter[i]))
    {
       aux=OptionFighter[i];
       abreviature += aux;
       i++;   
    } 
    if(abreviature.length()>0) // Se utiliza para ver si abreviature tiene nueva información almacenada
    {
         FighterType(ship,NumberFighter,abreviature,addedFighter);
    }
  }
  return(addedFighter);
}
//---------------------------------------------------
void initializeShip(Ship &ship,bool side)
{ 
  ship.credits=CREDITS_INITIAL;
  ship.wins=0;
  ship.losses=0;
  ship.side=side;

  if(side==REBEL)
  {
   ship.maxCapacity=REBSHIPCAPACITY;
   InitializeAndAdd(initialRebelShipDotation,ship);
  }
  else  // se hace lo mismo que con la nave rebelde solo que con los respectivos datos de la nave imperial.
  {
    ship.maxCapacity=IMPSHIPCAPACITY;
    InitializeAndAdd(initialImperialShipDotation,ship);
  } 
}


//---------------------------------------------------
void listFighter(const Fighter &f) // Muestra toda la información de un caza
{
  cout << f.type;
  cout << "  (v="<<f.velocity;
  cout << ", a="<<f.attack;
  cout << ", s="<<f.shield;
  cout << ", c="<<f.cost<<")"<<endl;
}

void listFighters(const vector<Fighter> &vf) // Muestra toda la información de los cazas de la nave rebelde o imperial.
{
  
   for(unsigned int i=0;vf.size()>i;i++) // se repite tantas veces como cazas hay en una nave  dando  la información de todos los cazas.
    {
      cout << "["<<i+1<<"] "<<vf[i].type;
      cout << "(v="<<vf[i].velocity;
      cout << ", a="<<vf[i].attack;
      cout << ", s="<<vf[i].shield;
      cout << ", c="<<vf[i].cost<<")"<<endl;  
    }
}

void listShip(const Ship &ship) // Muestra toda la información de la nave rebelde o imperial.
{
  cout << "Ship info: ";
  cout << "max. capacity="<<ship.maxCapacity<<", ";

  if(ship.side==IMPERIAL)
  {
    cout << "side=IMPERIAL, ";
  }
  else
  {
    cout << "side=REBEL, ";
  }
    cout << "credits="<<ship.credits<<", ";
    cout << "wins="<<ship.wins<<", ";
    cout << "losses="<<ship.losses<<endl;
    listFighters(ship.fighters);
}


//---------------------------------------------------
bool addFighter(Ship &ship) //función que se utiliza para añadir cazas a la nave imperial, si se añade satisfactoriamente regresa true
{
  string OptionFighter;
  cin.get();
  cout << " Enter fighters:";
  getline(cin,OptionFighter);
  return(InitializeAndAdd(OptionFighter,ship));
}
//-----------------------------------------------------
void checkPrize(const int &c, const Ship &ship, int &cost,const  int &OptionImprove, bool &enoughCredits)
{
  switch(OptionImprove) // dependiendo de la estadística que se eligió mejorar en improveFighter, se calcula el coste de la mejora.
  {
    case 'v': 
      cost = 2 * c;
      break;
    case 'a':
      cost = 3 * c;
      break;
    case 's':
      cost = (c+1)/2;
    break;
  }
  if (cost > ship.credits) 
  {
    enoughCredits = false;
  }
}
//---------------------------------------------------
void CostImprove(Ship &ship,const char &OptionImprove, const int &fighterNumber, const int &c, bool &improved) //calcula el coste de la mejora y se mejora la estadística del caza que se quiera.
{
  int cost; //almacena el coste de la mejora.
  char confirm; // almacena la opción del usuario cuando se le pide que confirme la mejora.
  bool enoughCredits = true;

  checkPrize(c,ship, cost, OptionImprove, enoughCredits);

  if (enoughCredits)
   { 
     cout << "That will cost you "<<cost<<" credits. Confirm? (y/n)";
     cin >> confirm;
    if(confirm=='y') // comprueba la confirmación, si es y, aplica la mejora, sino se sale al menú sin modificar el caza.
    {
      switch(OptionImprove)
        {
          case 'v':        
            ship.fighters[fighterNumber-1].velocity += c;
          break;
          case'a':            
            ship.fighters[fighterNumber-1].attack += c;         
          break;
          case 's':           
            ship.fighters[fighterNumber-1].shield += c;           
          break;
        }
        ship.fighters[fighterNumber-1].cost += cost; // se calcula el nuevo coste del caza.
        ship.credits -= cost; // se calcula la nueva cantidad de créditos disponibles.
        cout << "Fighter improved: ";
        listFighter(ship.fighters[fighterNumber-1]); 
        improved=true;
      }
    }
  else // si no hay suficientes créditos en la nave para la mejora, devuelve el mensaje "no funds" y regresa al menú.
  {
    error(NO_FUNDS);
  }
}

bool CorrectFighterNumber(const int &fighterNumber, const Ship &ship) //comprueba si el número de caza elegido por el usuario es correcto.
{
  bool CorrectNumber=false;

  if((fighterNumber>ship.fighters.size()) || (fighterNumber<=0)) 
  {
    error(WRONG_NUMBER);
  }
  else
  {
     CorrectNumber=true;
  }
  return(CorrectNumber);
}

bool improveFighter(Ship &ship)
{
  int fighterNumber, c; // fighter number es el caza seleccionado por el usuario y la variable c es la cantidad que se mejorará de v,a ò s.
  char OptionImprove; // es la estadística que se quiere mejorar (v/a/s).
  bool improved=false; // es la variable que devuelve si el caza ha sido mejorado o no.

  cout << " Select fighter number: ";
  cin >> fighterNumber;
  
    if(CorrectFighterNumber(fighterNumber,ship)) 
    {  

      cout << "What to improve (v/a/s)?";
      cin >> OptionImprove;


     if((OptionImprove=='a') ^ (OptionImprove=='v')  ^ (OptionImprove=='s') )
      {
        cout << " Amount: ";
        cin >> c;
        CostImprove(ship,OptionImprove,fighterNumber, c, improved);
      }
     else // si la opción de mejora  es incorrecta devuelve Unknown Option y se devuelve al menú.
      {
        error(UNKNOWN_OPTION);
      }
   }

      return(improved);
  }

//---------------------------------------------------
bool CorrectFighterNumbers(const string &fighterNumber, const Ship &ship, vector<int> &NumFighters) 
{ // comprueba que los números introducidos para el escuadrón sean correctos
  int auxNumbers; //guarda los números de los cazas para luego ver si pertenecen a la nave.
  bool CorrectFighters=false; // comprueba si al menos un  caza se ha añadido al escuadrón.
  string Number;// guarda un número para luego pasarlo a auxNumbers.
  int aux;// utilizado para guardar los valores de los cazas momentaneamente mientras se organizan de mayor a menor.

  for(int i=0; i<fighterNumber.length();i++)
  {
    if(fighterNumber[i]>'0' && fighterNumber[i]<='9')
    {
      Number=fighterNumber[i];
      auxNumbers=atoi(Number.c_str());
      if(fighterNumber[i+1]>='0' && fighterNumber[i+1]<='9')
      {
        Number=fighterNumber[i+1];
        auxNumbers= auxNumbers*10+atoi(Number.c_str());
        i++;
      }
      if(auxNumbers<=ship.fighters.size())
      {
        NumFighters.push_back(auxNumbers);
        CorrectFighters=true;
      }
    }
  }
  for(int j=0;j<NumFighters.size();j++)
  {
    for(int k=0;k<NumFighters.size()-1;k++)
    {
      if(NumFighters[k]<NumFighters[k+1])
      {
       aux=NumFighters[k];
       NumFighters[k]=NumFighters[k+1];
       NumFighters[k+1]=aux;
      }
    } 
  }
  return(CorrectFighters);
}
//---------------------------------------------------------------
bool FightSimulation(Fighter &fg1, Fighter &fg2, const int &n)
{
  int d; // es una variable que almacena el daño.
  int u; // almacena el umbral de ataque.(n>4 && n<96) // se comprueba el valor de n, si cumple la condición, sigue el combate, sino, termina en empate.
  bool endFight=false; // variable que devuelve true si la pelea ha sido interrumpida.
  const int f=300;

  if(n>4 && n<96)
  {
    u = (100*fg1.velocity)/(fg1.velocity+fg2.velocity);
    if(n<=u) // si u es mayor o igual que n, el caza imperial ataca, sino, el caza rebelde ataca.
    { 
      d=(n*fg1.attack)/f;
      fg2.shield -= d;
    }
    else if (n > u)
    {
      d=((100-n)*fg2.attack)/f;
      fg1.shield -= d;
    }
  }
  else
  {
    endFight=true;
  }
  return(endFight);
}
//---------------------------------------------------
int fight(Fighter &fg1,Fighter &fg2) 
{
  int n; // numero aleatorio generado para la pelea.
  int resultFight=0;
  int fightEnd=false; // variable para verificar si la pelea no ha sido interrumpida.
  
  cout << "-- begin fight"<<endl;
  listFighter(fg1);
  listFighter(fg2);
  while(fg1.shield>0 && fg2.shield>0 && fightEnd==false) // mientras los dos cazas no hayan sido destruidos y no haya sido interrumpida, continua la pelea.
  {
   n=getRandomNumber(100);
   fightEnd=FightSimulation(fg1,fg2,n);
  }

      cout << "--"<<endl;
      listFighter(fg1);
      listFighter(fg2);
      cout << "-- end fight"<<endl;
      if(fg1.shield<=0) // se mira cual ha sido el caza ganador, si ha ganado fg1, la función devolverá -1, si gana fg2, -1, y sino ninguno se destuye, devuelve 0.
      {
        resultFight=1;
      }
      else if(fg2.shield<=0)
      {
        resultFight=-1;
      }
      return(resultFight);
}
//---------------------------------------------------
void FightResults(Ship &imperial,Ship &rebel,const Fighter &fg1, const Fighter &fg2, const int &result) // dependiendo del resultado del módulo fight, se modifica la información de un caza o de otro.
{

  switch(result)
  {
    case -1 : // si el imperial ha ganado, se devuelve a la nave, se recompensa con el coste del caza enemigo y se aumentan las victorias del imperial y las derrotas del rebelde.
    imperial.credits += fg2.cost;
    imperial.wins += 1;
    rebel.losses +=1;
    break;
    case 1 :// si el rebelde ha ganado, se devuelve a su nave, se recompensa con el coste del caza enemigo y se aumentan las victorias del caza rebelde y las derrotas del imperial.
    rebel.credits += fg1.cost;
    rebel.wins += 1;
    imperial.losses +=1;
    break;
    case 0 : // si ha terminado en empate, se devuelven los cazas a sus respectivas naves.
    break;
  }
}
//---------------------------------------------------
void ChoosingFighters(vector <Fighter> &Imperial, vector <Fighter> &Rebel,Ship &ShipImp,Ship &ShipReb)
{
  int result; // variable que da el resultado de la batalla (1,0,-1)
  bool EndBattle=false; // saber si la pelea se ha acabado
  Fighter fg1,fg2; // guarda los cazas elegidos.
  int aux,n; // usado para guardar números aleatorios.

  cout << "Squadron A"<<endl;
  listFighters(Imperial);
  cout << "Squadron B"<<endl;
  listFighters(Rebel);
  cout << "------"<<endl;

  while(EndBattle==false)
  {
    n=getRandomNumber(100);
   if(n>=10 && Imperial.size()>0 && Rebel.size()>0)
    {
      aux=getRandomNumber(Imperial.size());
      fg1=Imperial[aux];
      Imperial.erase(Imperial.begin()+aux);
      aux=getRandomNumber(Rebel.size());
      fg2=Rebel[aux];
      Rebel.erase(Rebel.begin()+aux);
      result=fight(fg1,fg2);
      FightResults(ShipImp,ShipReb,fg1,fg2,result);
      if(fg1.shield>0)
      {
        Imperial.push_back(fg1);        
      }
      if(fg2.shield>0)
      {
        Rebel.push_back(fg2);
      }
    }
    else
    {
      EndBattle=true;
    }
  }
  cout <<"------"<<endl;
  cout << "Squadron A"<<endl;
  listFighters(Imperial);
  cout << "Squadron B"<<endl;
  listFighters(Rebel);
}

//---------------------------------------------------------
void ReturnSquad(vector <Fighter> &Imperial, vector <Fighter> &Rebel,Ship &ShipImp,Ship &ShipReb)
{
  for(int i=0;i<Imperial.size();i++)
  {
    ShipImp.fighters.push_back(Imperial[i]);
  }
  for(int j=0;j<Rebel.size();j++)
  {
    ShipReb.fighters.push_back(Rebel[j]);
  }
}
//---------------------------------------------------------
void ChooseRebels(Ship &rebel, const vector<Fighter> &ChosenFighters, vector<Fighter> &ChosenRebels)
{
  int size;
  int Margen = getRandomNumber(7)-3; // el tamaño del escuadrón rebelde con respecto al escuadrón imperial
 
  size=Margen+ChosenFighters.size();
  
  if(size>rebel.fighters.size())
  {
    size=rebel.fighters.size();
  }
  if(size<=0)
  {
    size=1;
  }
  for(int i=0; i<size;i++)
  {
    int c=getRandomNumber(rebel.fighters.size());
    ChosenRebels.push_back(rebel.fighters[c]); // escoger un número aleatorio de la nave rebelde y meterlo en el escuadrón
    rebel.fighters.erase(rebel.fighters.begin()+c);  
  }
}
//---------------------------------------------------------
void launchFighter(Ship &imperial,Ship &rebel)
{
  vector<Fighter> ChosenRebels; // escuadrón de cazas rebeldes.
  vector<int> NumFighters; // utilizado para almacenar los números de caza de mayor a menor.
  string fighterNumber; // número del caza elegido por el usuario.
  vector<Fighter> ChosenFighters; // vector donde se almacenan los cazas imperiales elegidos.

  if((imperial.fighters.size()<=0) || (rebel.fighters.size()<=0)) //si no quedan cazas imperiales o rebeldes, sale el mensaje NO FIGHTERS y regresa al menú
  {
    error(NO_FIGHTERS);
  }
  else
  {
    cout <<"Select fighter numbers:";
    cin.get();
    getline(cin,fighterNumber);
    if(CorrectFighterNumbers(fighterNumber,imperial,NumFighters)) // se comprueba si el número introducido por el usuario es correcto, si no lo es se sale al menú.
    {
      for(int i=0;i<NumFighters.size();i++)
      {
        ChosenFighters.push_back(imperial.fighters[NumFighters[i]-1]); // se añaden a Chosen Fighters los cazas escogidos por el usuario.
        imperial.fighters.erase(imperial.fighters.begin()+NumFighters[i]-1); // se eliminan de la nave los cazas escogidos por el usuario.
      } 
      ChooseRebels(rebel,ChosenFighters,ChosenRebels);
      ChoosingFighters(ChosenFighters,ChosenRebels,imperial,rebel);
      ReturnSquad(ChosenFighters,ChosenRebels,imperial,rebel);
    } 
    else
    {
      error(WRONG_NUMBER);
    }
  }
}

//---------------------------------------------------
void menu()
{
  cout << "1- List ship info"<<endl;
  cout << "2- Add fighters"<<endl;
  cout << "3- Repair/improve fighter"<<endl;
  cout << "4- Launch squadron"<<endl;
  cout << "5- List enemy info"<<endl;
  cout << "s- Save data"<<endl;
  cout << "S- Save enemy data"<<endl;
  cout << "l- Load data"<<endl;
  cout << "L- Load enemy data"<<endl;
  cout << "x- Export data"<<endl;
  cout << "i- Import data"<<endl;
  cout << "q- Quit"<<endl;
  cout << "Option:";
}


//---------------------------------------------------

int main(int argc,char *argv[])
{
   Ship imperialDestroyer, rebelShip;
   char option;
   
   srand(1); // inicializar secuencia de números aleatorios: NO TOCAR

   initializeShip(imperialDestroyer,IMPERIAL); 
   initializeShip(rebelShip,REBEL);

  do  
   {
    menu();
    cin >> option;
      switch (option) 
      {
        case '1': 
          listShip(imperialDestroyer);
          break;
        case '2':
          addFighter(imperialDestroyer);
          break;
        case '3': 
           improveFighter(imperialDestroyer);
          break;
        case '4': 
          launchFighter(imperialDestroyer,rebelShip);
          break;
        case '5': 
          listShip(rebelShip);
          break;
        case 'q':
          break;
        default: 
          error(UNKNOWN_OPTION);           
      }

   } while (option != 'q');
}
