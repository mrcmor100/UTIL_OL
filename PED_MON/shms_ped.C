//This macro reads the SHMS histograms from Rootfile and reads the values of pedestals in each PMTs for, trigger, Hodoscopes, calorimeter, HGCER,  NGCERand Aerogel.
const Int_t goldRunNum1  = 2467;// for trig only
const Int_t goldRunNum   = 2687;// for detectors only
ofstream myfile;

void trig(Int_t, Int_t);
void hodo(Int_t, Int_t);
void calprshwr(Int_t, Int_t);
void calshwr(Int_t, Int_t);
void hgcer(Int_t, Int_t);
void ngcer(Int_t, Int_t);
void aero(Int_t, Int_t);

void shms_ped(Int_t runNumber, Int_t eventNumber)//MAIN FUNCTION
{
  const Int_t run          = runNumber;
  const Int_t nevents      = eventNumber;

  trig(runNumber, eventNumber);
  hodo(runNumber, eventNumber);
  calprshwr(runNumber, eventNumber);
  calshwr(runNumber, eventNumber);
  hgcer(runNumber, eventNumber);
  ngcer(runNumber, eventNumber);
  aero(runNumber, eventNumber);
 
}

void trig(Int_t runNumber, Int_t eventNumber)
{ 

  const Int_t run          = runNumber;
  const Int_t nevents      = eventNumber;
  const Int_t numHistos    = 5;     //Number of histos for Trig only

  myfile.open("outSHMS.txt");

  TFile *f1 = new TFile(Form("ROOTfiles/shms_replay_production_%d_50000.root", goldRunNum1));//Golden Run for Trigger only
  myfile <<"___________________________________________________________________________"<<endl;
  myfile <<"SHMS TRIGGER GOLDEN  RUN NUMBER    =" "  "<<goldRunNum1<<endl;
  myfile <<"SHMS TRIGGER CURRENT RUN NUMBER    =" "  "<<run<<endl;
  myfile <<"___________________________________________________________________________"<<endl;
  myfile <<"shms_sum_pped"<< "\t\t\t"<<"Mean_Gold"<<"\t"<<"Mean_Curr"<<"\t"<<"Mean_Diff"<<endl;
  myfile <<"___________________________________________________________________________"<<endl;


  TH1D *histG[numHistos];
  TH1D *hist[numHistos];
  Int_t channel[5] = {1,2,3,4,5}; //(see hallc_replay/MAPS/SHMS/DETEC/TRIG/ptrig.map)
  TString variables[numHistos] = {"pHGCER", "pNGCER", "pPSHWR", "pHG_MOD", "pNG_MOD"};
  TString histos[numHistos]    = {"ptrig_phgc_sum_pped", "ptrig_pngc_sum_pped", "ptrig_prshwr_sum_pped", "ptrig_phgc_sum_MOD_pped", "ptrig_pngc_sum_MOD_pped"};
 
  //Int_t plane=1;
  Int_t bar [5]={2,3,4,6,7}; //variables are corresponding to these bars
 
  // For Goldern

  double meanG[numHistos],    rmsG[numHistos];
  double mean[numHistos],     rms[numHistos];
  double meanDiff[numHistos], rmsDiff[numHistos];
  double eff_rms[numHistos];


  for (int i = 0; i < numHistos-2 ; i++)
    {
      histG[i] = (TH1D*)gDirectory->Get(histos[i]);
      if (histG[i]) 
        {
          meanG[i]=histG[i]->GetMean();
          rmsG[i]=histG[i]->GetRMS();
        }
      else 
        {        
          cout << "****************ATTENTION REQUIRED *****************************" << endl;
          cout << "************SHMS_TRIGGER_PULSE_PEDESTALS_HAVE PROBLEMS *********" << endl;
          cout << "************PLEASE LOOK AT SHMS_TRIGGER_SUM_PPED_HISTOGRAMS*****" << endl;    
        }
    }
 
  TFile *f2= new TFile(Form("ROOTfiles/shms_replay_production_%d_%d.root",run,nevents));  // no golden

  for (int i = 0; i < numHistos-2 ; i++)
    {
      hist[i] = (TH1D*)gDirectory->Get(histos[i]);
      // cout << "mean = " << hist[i]->GetMean() << endl;
      mean[i]=hist[i]->GetMean();
      rms[i]=hist[i]->GetRMS();
    }

  for (int i = 0; i < numHistos-2 ; i++)
    {
      meanDiff[i]=fabs(meanG[i]-mean[i]);
      eff_rms[i] = sqrt(pow(rmsG[i], 2)+pow(rms[i], 2));
    }


  for (int i = 0; i < numHistos-2 ; i++)
    {
      //open this cout if we want to print values in terminal
      //  cout<<    setprecision(2) << fixed  <<histos[i]<<"\t\t"<<" Mean_Golden  ="<<meanG[i]<<"\t\t"<<"Mean_Current  ="<<mean[i]<<"\t\t"<<"Mean_Diff  ="<<meanDiff[i]<<endl;

      //cout <<channel[i]<<""<<"\t"<< bar[i]<<"\t"<<histos[i]<<"\t"<<"\t"<<meanG[i]<<"\t\t"<<mean[i]<<"\t\t"<<meanDiff[i]<<endl;
      myfile << setprecision(2) << fixed <<histos[i]<<"\t"<<"\t"<<meanG[i]<<"\t\t"<<mean[i]<<"\t\t"<<meanDiff[i]<<endl;//to store values in .txt file

    }
 
  for (int i = 0; i < numHistos-2 ; i++)
    {
      if(abs(meanDiff[i])>2.5) //The following message will appear in terminal if Pedestal mean differnce is greater than 2.5
        {
          
          cout<<"_____________________________________________________________________"<<endl;
          cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!" << endl;
          cout<<"_____________________________________________________________________"<<endl;
         
          cout << "--------------------See ROC2 --- SLOT 14---------------------------" << endl;//hallc_replay/MAPS/SHMS/DETEC/TRIG/ptrig.map
          cout<<"___________________________________________________________________________________"<<endl;
          cout<<"Golden Run = "<<goldRunNum1<<"     "<<"AND""     "<<"This Run = "<<run<<endl;
          cout<<"___________________________________________________________________________________"<<endl;
          cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
          cout<<"___________________________________________________________________________________"<<endl;
          cout<<" HERE THE MEAN DIFFERNCE OF PULSE PEDUSTAL BETWEEN GOLDEN RUN (MG) AND"<<endl;
          cout<<"CURRENT RUN (M) IS GREATER THAN 2.5 mV FOR THE FOLLOWING HISTOGRAM(S)"<<endl;
          cout<<"PLEASE CONSULT WITH BRAD SAWATZKY (brads@jlab.org) or ERIC POOSER(pooser@jlab.org)"<<endl;
          cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
          cout<<"___________________________________________________________________________________"<<endl;
          cout << setprecision(2) << fixed <<"chan"<<" "<< channel[i] << " " <<"bar"<<" "<< bar[i] 
               << "\t" <<  variables[i] << "\t" <<"MG"<<"  "<<meanG[i] <<"  " 
               <<"M"<<"  "<< mean[i] <<" " <<"MD"<<" "<< meanDiff[i] << endl;//MG=mean golden,M = mean current, MD = mean diff
          cout<<"_____________________________________________________________________"<<endl;
       
        }
    }

  myfile <<"___________________________________________________________________________"<<endl;
  //myfile.close(); 
}

//--------------------------------FOR SHMS  DETECTORS ------------STARTS FROM HERE-------------------

 
void hodo(Int_t runNumber, Int_t eventNumber)
{
  const Int_t run      = runNumber;
  const Int_t nevents  = eventNumber;

  Double_t mean1[122];
  Double_t mean11[122];
  Double_t meanDiff1[122];
  TFile *f3 = new TFile(Form("ROOTfiles/shms_replay_production_%d_50000.root",goldRunNum));//Golden ROOTfile Detectors only
  TFile *f4 = new TFile(Form("ROOTfiles/shms_replay_production_%d_%d.root",run,nevents));//Takes Current ROOTfile

  myfile <<"SHMS PEDESTAL GOLDEN  RUN NUMBER  =" "    "<<goldRunNum<<endl;
  myfile <<"SHMS PEDESTAL CURRENT  RUN NUMBER =" "    "<<run<<endl;
  myfile <<"____________________________________________________________________________________________"<<endl;
  myfile <<"shms_good_pped"<< "\t\t\t\t"<<"PMT_Num"<<"\t\t"<<"Mean_Gold"<<"    "<<"Mean_Curr"<<"    "<<"Mean_Diff"<<endl;
  myfile <<"____________________________________________________________________________________________"<<endl;

 

  //******************FOR SHMS HODOSCOPE, PMTS in 1x = 13, 1y =13, 2x=14, 2y =21*************************

 
  // TH1D *hist[8];//8 Histograms for SHMS HODO
  // TH1D *hist1[8];

  TString plane1[4]= {"1x","1y","2x","2y"};//4-planes
  TString polarity1[2] = {"pos","neg"};//2-sides in each plane
  Int_t numPMT1[4] = {13, 13, 14, 21};//Pmts in 4 planes
  
  for(int i = 0; i < 4; ++i)//plane
    {
      for(int j = 0; j<2; ++j)//polarity
        {
          TString hist1Name = "phodo_";
          hist1Name += plane1[i];
          hist1Name += "_good_pped_vs_pmt_";
          hist1Name += polarity1[j]; //READS THE HISTOGRAM for GOLDEN RUN
            
          TH2D* hist1_2D = (TH2D*)f3->Get(hist1Name);
              
          TString hist1Name1 = "phodo_";  //Get The Histogram
          hist1Name1 += plane1[i];
          hist1Name1 += "_good_pped_vs_pmt_";
          hist1Name1 += polarity1[j]; //READS THE HISTOGRAMFOR CURRENT RUN
          
          TH2D *hist11_2D  = (TH2D*)f4->Get(hist1Name1);//Read 
          TH1D *projHist1;
          TH1D *projHist11;

          bool first_error = true;
              
          for(int k = 0; k< numPMT1[i]; ++k)//PMT
              
            {
              projHist1     =  hist1_2D->ProjectionY("phodo_proj", k+1, k+1);
              projHist11    =  hist11_2D->ProjectionY("phodo_proj1", k+1, k+1);//CALCULATES Y-PROJECTION

              mean1[k] = projHist1->GetMean();//Mean Golden
              mean11[k] = projHist11->GetMean();//Mean Current 
              meanDiff1[k] = fabs(mean1[k]-mean11[k]);//Mean Diff

	      //  cout <<  setprecision(2) << fixed<< hist1Name<< " PMT = "<< k+1 <<"  Mean_Golden = "<<mean1[k]<<  " & " << "Mean_Current ="<<mean11[k]<<" Mean_Diff = "<<fabs(mean1[k]-mean11[k])<<endl;
              myfile << setprecision(2) << fixed <<"-----"<< hist1Name << "\t"<<"PMT ="<<"\t"<<k+1<<"\t"<<mean1[k]<<"\t\t"<< mean11[k]<<"\t\t"<< meanDiff1[k]<<endl;//to store values in .txt file
                  
              if (mean11[k]!=0.0) {

                if(fabs(meanDiff1[k])>2.5) //The following message will appear in terminal if Pedestal mean differnce is greater than 2.5 mV
                  {
                        
                    if(first_error){
                      cout<<"___________________________________________________________________________________"<<endl;
                      cout<<"Golden Run = "<<goldRunNum<<"     "<<"AND""     "<<"This Run = "<<run<<endl;
                      cout<<"___________________________________________________________________________________"<<endl;
                      cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                      cout<<"___________________________________________________________________________________"<<endl;
                      cout<<" HERE THE MEAN DIFFERNCE OF PULSE PEDUSTAL BETWEEN GOLDEN RUN (MG) AND"<<endl;
                      cout<<"CURRENT RUN (M) IS GREATER THAN 2.5 mV FOR THE FOLLOWING HISTOGRAM(S)"<<endl;
                      cout<<"PLEASE CONSULT WITH BRAD SAWATZKY (brads@jlab.org) or ERIC POOSER(pooser@jlab.org)"<<endl;
                      cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                      cout<<"___________________________________________________________________________________"<<endl;
                      first_error=false;
                    }
                    cout << setprecision(2) << fixed<<hist1Name<<"   "<<"PMT :"<<k+1<<"   "<<"MG ="<<"  "<<mean1[k] <<"  " 
                         <<"M ="<<"  "<< mean11[k] <<" " <<"MD"<<" "<< meanDiff1[k] << endl;//MG=mean golden,M = mean current, MD = mean diff
                    cout<<"___________________________________________________________________________________"<<endl;
                  }
              }
            }
        }
    }
  
  myfile<<"**********************************************************************************************"<<endl;
    
}//end hodo

//  //******************************SHMS CAL PRSHWR*******************************************************
void calprshwr(Int_t runNumber, Int_t eventNumber)
{   

  const Int_t run      = runNumber;
  const Int_t nevents  = eventNumber;
  TFile *f3 = new TFile(Form("ROOTfiles/shms_replay_production_%d_50000.root",goldRunNum));//Golden ROOTfile Detectors only
  TFile *f4 = new TFile(Form("ROOTfiles/shms_replay_production_%d_%d.root",run,nevents));//Takes Current ROOTfileDouble_t mean1[1000];
  Double_t mean2[28];
  Double_t mean22[28];
  Double_t meanDiff2[28];
 

  TString polarity2[2] = {"pos","neg"};//Two sides 
  Int_t numPMT2[2] = {14, 14};//14 Pmts in each side
  
  for(int j = 0; j<2; ++j)//polarity
    {
      TString hist2Name = "pcal_prshwr_good_pped_vs_pmt_";
      hist2Name += polarity2[j];//Reads Histogram from Golden Run
      TH2D* hist2_2D = (TH2D*)f3->Get(hist2Name);

      TString hist2Name1 = "pcal_prshwr_good_pped_vs_pmt_";
      hist2Name1 += polarity2[j];//Reads Histogram from Current Run
      TH2D *hist22_2D  = (TH2D*)f4->Get(hist2Name1);

      TH1D *projHist2;
      TH1D *projHist22;
       
      bool first_error = true;
              
      for(int k = 0; k< numPMT2[j]; ++k)//PMT
        {
          projHist2     =  hist2_2D->ProjectionY("pcal_proj", k+1, k+1);//Does Y-Projectin
          projHist22    =  hist22_2D->ProjectionY("pcal_proj1", k+1, k+1);

          mean2[k] = projHist2->GetMean();//Mean Golden
          mean22[k] = projHist22->GetMean();//Mean Current
          meanDiff2[k] = fabs(mean2[k]-mean22[k]);//Mean Diff
                     
	  //  cout <<  setprecision(2) << fixed <<hist2Name<< " PMT = "<< k+1 <<"  Mean_Golden = "<<mean2[k]<<  " & " << "Mean_Current ="<<mean22[k]<<" Mean_Diff = "<<meanDiff2[k]<<endl;
          myfile << setprecision(2) << fixed << hist2Name << "\t"<< "PMT ="<<"\t"<<k+1<<"\t"<<mean2[k]<<"\t\t"<< mean22[k]<<"\t\t"<< meanDiff2[k]<<endl;//to store values in .txt file


          if (mean22[k]!=0.0) {

            if(fabs(meanDiff2[k])>2.5)//setting 2.5 mV as a cutoff point
              {
                if(first_error){
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<"Golden Run = "<<goldRunNum<<"     "<<"AND""     "<<"This Run = "<<run<<endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<" HERE THE MEAN DIFFERNCE OF PULSE PEDUSTAL BETWEEN GOLDEN RUN (MG) AND"<<endl;
                  cout<<"CURRENT RUN (M) IS GREATER THAN 2.5 mV FOR THE FOLLOWING HISTOGRAM(S)"<<endl;
                  cout<<"PLEASE CONSULT WITH BRAD SAWATZKY (brads@jlab.org) or ERIC POOSER(pooser@jlab.org)"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  first_error=false;
                }
                cout << setprecision(2) << fixed<<hist2Name<<"   "<<"PMT :"<<k+1<<"   "<<"MG ="<<"  "<<mean2[k] <<"  " 
                     <<"M ="<<"  "<< mean22[k] <<" " <<"MD"<<" "<< meanDiff2[k] << endl;//MG=mean golden,M = mean current, MD = mean diff
                cout<<"___________________________________________________________________________________"<<endl;
              }
            
          }
        }
    }
    
  myfile<<"**********************************************************************************************"<<endl;

}// calprshwr
//****************************SHMS CAL SHWR 1 HISTOGRAM ONLY*****************************************


void calshwr(Int_t runNumber, Int_t eventNumber)
{ 
 
  const Int_t run      = runNumber;
  const Int_t nevents  = eventNumber;
  TFile *f3 = new TFile(Form("ROOTfiles/shms_replay_production_%d_50000.root",goldRunNum));//Golden ROOTfile Detectors only
  TFile *f4 = new TFile(Form("ROOTfiles/shms_replay_production_%d_%d.root",run,nevents));//Takes Current ROOTfile
  Double_t mean3[224];
  Double_t mean33[224];
  Double_t meanDiff3[224];
  // TH1D *hist[1];//1 histogram
  // TH1D *hist1[1];
  TString pcal[1]= {"pcal_shwr"};
  Int_t numPMT3[1] = {224};//224 pmts

  for(int i = 0; i < 1; ++i)//plane
    {

      TString hist3Name = "pcal_shwr";
      hist3Name += "_good_pped_vs_pmt";
      TH2D* hist3_2D = (TH2D*)f3->Get(hist3Name);//Read Histogram from Golden Run
        
      TString hist3Name1 = "pcal_shwr";
      hist3Name1 += "_good_pped_vs_pmt";
      TH2D *hist33_2D  = (TH2D*)f4->Get(hist3Name1);//Reads Histogram from Current Run
                
      TH1D *projHist3;
      TH1D *projHist33;
      bool first_error = true;

      for(int k = 0; k< numPMT3[i]; ++k)//PMT
        {
          projHist3     =  hist3_2D->ProjectionY("hcal_proj1", k+1, k+1);
          projHist33    =  hist33_2D->ProjectionY("hcal_proj11", k+1, k+1);//Y-projection

          mean3[k] = projHist3->GetMean();
          mean33[k] = projHist33->GetMean();
          meanDiff3[k] = fabs(mean3[k]-mean33[k]);//Taking Absolute mean differnce

	  //  cout <<  setprecision(2) << fixed<< hist3Name<< " " " PMT =  "<< k+1<< "\t"<< "Mean_Golden =  "<<mean3[k]<<  " & " << "Mean_Current = "<<mean33[k]<<" Mean_Diff = "<<meanDiff3[k]<<endl;

          myfile << setprecision(2) << fixed <<"----"<< hist3Name << "\t\t"<< "PMT ="<<"\t"<<k+1<<"\t"<<mean3[k]<<"\t\t"<< mean33[k]<<"\t\t"<< meanDiff3[k]<<endl;//to store values in .txt file

          if (mean33[k]!=0.0){

            if (fabs(meanDiff3[k])>2.5 )//The following message will appear in terminal if Pedestal mean differnce is greater than 2.5
              {
                if(first_error){
                  cout<<"__________________________________________________________________ ______________"<<endl;
                  cout<<"Golden Run = "<<goldRunNum<<"     "<<"AND""     "<<"This Run = "<<run<<endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<" HERE THE MEAN DIFFERNCE OF PULSE PEDUSTAL BETWEEN GOLDEN RUN (MG) AND"<<endl;
                  cout<<"CURRENT RUN (M) IS GREATER THAN 2.5 mV FOR THE FOLLOWING HISTOGRAM(S)"<<endl;
                  cout<<"PLEASE CONSULT WITH BRAD SAWATZKY (brads@jlab.org) or ERIC POOSER(pooser@jlab.org)"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"____________________________________________________________________________________"<<endl;
                  first_error=false;
                }
              
                cout << setprecision(2) << fixed<<hist3Name<<"   "<<"PMT :"<<k+1<<"   "<<"MG ="<<"  "<<mean3[k] <<"  " 
                     <<"M ="<<"  "<< mean33[k] <<" " <<"MD"<<" "<< meanDiff3[k] << endl;//MG=mean golden,M = mean current, MD = mean diff
                cout<<"___________________________________________________________________________________"<<endl;
              }
          } 
        }
    }
   

  myfile <<"************************************************************************************************"<<endl;

}//end calshwr
//********************SHMS HGCER************************1 Histogram 4 PMTs**********************************
void hgcer(Int_t runNumber, Int_t eventNumber)


{  
  const Int_t run      = runNumber;
  const Int_t nevents  = eventNumber;
  TFile *f3 = new TFile(Form("ROOTfiles/shms_replay_production_%d_50000.root",goldRunNum));//Golden ROOTfile Detectors only
  TFile *f4 = new TFile(Form("ROOTfiles/shms_replay_production_%d_%d.root",run,nevents));//Takes Current ROOTfile
  Double_t mean4[4];
  Double_t mean44[4];
  Double_t meanDiff4[4];

  Int_t numPMT4[1] = {4};//4 Pmts 

  for(int j = 0; j<1; ++j)//polarity
    {
      TString hist4Name = "phgcer_good_pped_vs_pmt";//Read the histogram from Golden Run
      TH2D* hist4_2D = (TH2D*)f3->Get(hist4Name);
          
      TString hist4Name1 = "phgcer_good_pped_vs_pmt";  //Get The Histogram from Current Run
      TH2D *hist44_2D  = (TH2D*)f4->Get(hist4Name1);//
      TH1D *projHist4;
      TH1D *projHist44;

      bool first_error = true;
              
      for(int k = 0; k< numPMT4[j]; ++k)//PMT
        {
          projHist4    =   hist4_2D->ProjectionY("phgcer_proj", k+1, k+1);//Does Y-projection
          projHist44   =   hist44_2D->ProjectionY("phgcer_proj1", k+1, k+1);

          mean4[k] = projHist4->GetMean();//Mean Golden
          mean44[k] = projHist44->GetMean();//Mean Current
          meanDiff4[k] =fabs(mean4[k]-mean44[k]);//Takes mean diff
          //cout << setprecision(2) << fixed<<mean[k]<<"  "<<mean1[k]<<" "<<fabs(meanDiff[k])<<endl;
	  // cout <<  setprecision(2) << fixed<< hist4Name<< " " " PMT = "<< k+1 <<"""Mean_Golden = "<<mean4[k]<<  " & " << "Mean_Current = "<<mean44[k]<<" Mean_Diff = "<<meanDiff4[k]<<endl;


          myfile << setprecision(2) << fixed <<"-------"<< hist4Name << "\t\t"<< "PMT ="<<"\t"<<k+1<<"\t"<<mean4[k]<<"\t\t"<< mean44[k]<<"\t\t"<< meanDiff4[k]<<endl;//to store values in .txt file
          if (mean44[k]!=0.0){
            if(fabs(meanDiff4[k])>2.5) //The following message will appear in terminal if Pedestal mean differnce is greater than 2.5
         
              {
                if(first_error){
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<"Golden Run = "<<goldRunNum<<"     "<<"AND""     "<<"This Run = "<<run<<endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<" HERE THE MEAN DIFFERNCE OF PULSE PEDUSTAL BETWEEN GOLDEN RUN (MG) AND"<<endl;
                  cout<<"CURRENT RUN (M) IS GREATER THAN 2.5 mV FOR THE FOLLOWING HISTOGRAM(S)"<<endl;
                  cout<<"PLEASE CONSULT WITH BRAD SAWATZKY (brads@jlab.org) or ERIC POOSER(pooser@jlab.org)"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  first_error=false;
                }
                cout << setprecision(2) << fixed<<hist4Name<<"   "<<"PMT :"<<k+1<<"   "<<"MG ="<<"  "<<mean4[k] <<"  " 
                     <<"M ="<<"  "<< mean44[k] <<" " <<"MD"<<" "<< meanDiff4[k] << endl;//MG=mean golden,M = mean current, MD = mean diff
                cout<<"___________________________________________________________________________________"<<endl;
              }
          }
        }
    }
  myfile <<"************************************************************************************************"<<endl;
        
}
//*************************SHMS NGCER 1 Histogram, 4 PMTs******************************************************
void ngcer(Int_t runNumber, Int_t eventNumber)


{  

  const Int_t run      = runNumber;
  const Int_t nevents  = eventNumber;
  TFile *f3 = new TFile(Form("ROOTfiles/shms_replay_production_%d_50000.root",goldRunNum));//Golden ROOTfile Detectors only
  TFile *f4 = new TFile(Form("ROOTfiles/shms_replay_production_%d_%d.root",run,nevents));//Takes Current ROOTfile
  Double_t mean5[4];
  Double_t mean55[4];
  Double_t meanDiff5[4];

  Int_t numPMT5[1] = {4};//4 Pmts

  for(int j = 0; j<1; ++j)//polarity
    {
      TString hist5Name = "pngcer_good_pped_vs_pmt";
      TH2D* hist5_2D = (TH2D*)f3->Get(hist5Name);//GET Hiistogram from Golden Run
          
      TString hist5Name1 = "pngcer_good_pped_vs_pmt";  //Get The Histogram from current Run
      TH2D *hist55_2D  = (TH2D*)f4->Get(hist5Name1);
      TH1D *projHist5;
      TH1D *projHist55;

      bool first_error = true;
              
      for(int k = 0; k< numPMT5[j]; ++k)//PMT
        {
                  
          projHist5     =  hist5_2D->ProjectionY("phgcer_proj", k+1, k+1);
          projHist55    =  hist55_2D->ProjectionY("phgcer_proj1", k+1, k+1);

          mean5[k] = projHist5->GetMean();//Mean Golden
          mean55[k] = projHist55->GetMean();//Mean Current
          meanDiff5[k] = fabs(mean5[k]-mean55[k]);//Mean Diff

	  //   cout <<  setprecision(2) << fixed<< hist5Name<< " " " PMT = "<< k+1 <<"""Mean_Golden = "<<mean5[k]<<  " & " << "Mean_Current = "<<mean55[k]<<" Mean_Diff = "<< meanDiff5[k]<<endl;

          myfile << setprecision(2) << fixed <<"--------"<< hist5Name << "\t\t"<< "PMT ="<<"\t"<<k+1<<"\t"<<mean5[k]<<"\t\t"<< mean55[k]<<"\t\t"<< meanDiff5[k]<<endl;//to store values in .txt file

          if (mean55[k]!=0.0){

            if(fabs(meanDiff5[k])>2.5) //The following message will appear in terminal if Pedestal mean differnce is greater than 2.5
         
              {
                if(first_error){
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<"Golden Run = "<<goldRunNum<<"     "<<"AND""     "<<"This Run = "<<run<<endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<" HERE THE MEAN DIFFERNCE OF PULSE PEDUSTAL BETWEEN GOLDEN RUN (MG) AND"<<endl;
                  cout<<"CURRENT RUN (M) IS GREATER THAN 2.5 mV FOR THE FOLLOWING HISTOGRAM(S)"<<endl;
                  cout<<"PLEASE CONSULT WITH BRAD SAWATZKY (brads@jlab.org) or ERIC POOSER(pooser@jlab.org)"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  first_error=false;
                }
                cout << setprecision(2) << fixed<<hist5Name<<"   "<<"PMT :"<<k+1<<"   "<<"MG ="<<"  "<<mean5[k] <<"  " 
                     <<"M ="<<"  "<< mean55[k] <<" " <<"MD"<<" "<< meanDiff5[k] << endl;//MG=mean golden,M = mean current, MD = mean diff
                cout<<"___________________________________________________________________________________"<<endl;
              }
          }
        }
    }
    
  myfile <<"************************************************************************************************"<<endl;
}

//******************************SHMS AEROGEL 7 PMTS EACH*******************************************************
void aero(Int_t runNumber, Int_t eventNumber)
{
  const Int_t run      = runNumber;
  const Int_t nevents  = eventNumber;
  TFile *f3 = new TFile(Form("ROOTfiles/shms_replay_production_%d_50000.root",goldRunNum));//Golden ROOTfile Detectors only
  TFile *f4 = new TFile(Form("ROOTfiles/shms_replay_production_%d_%d.root",run,nevents));//Takes Current ROOTfile
  Double_t mean6[14];
  Double_t mean66[14];
  Double_t meanDiff6[14];

  TString polarity6[2] = {"pos","neg"};//Two sides 
  Int_t numPMT6[2] = {7, 7};//14 Pmts in each side
  
  for(int j = 0; j<2; ++j)//polarity
    {
      TString hist6Name = "paero_good_pped_vs_pmt_";
      hist6Name += polarity6[j];//Reads Histogram from Golden Run
      TH2D* hist6_2D = (TH2D*)f3->Get(hist6Name);

      TString hist6Name1 = "paero_good_pped_vs_pmt_";
      hist6Name1 += polarity6[j];//Reads Histogram from Current Run
      TH2D *hist66_2D  = (TH2D*)f4->Get(hist6Name1);

      TH1D *projHist6;
      TH1D *projHist66;
       
      bool first_error = true;
              
      for(int k = 0; k< numPMT6[j]; ++k)//PMT
        {
          projHist6     =  hist6_2D->ProjectionY("paero_proj", k+1, k+1);//Does Y-Projectin
          projHist66    =  hist66_2D->ProjectionY("paero_proj1", k+1, k+1);

          mean6[k] = projHist6->GetMean();//Mean Golden
          mean66[k] = projHist66->GetMean();//Mean Current
          meanDiff6[k] = fabs(mean6[k]-mean66[k]);//Mean Diff
                     
	  //  cout <<  setprecision(2) << fixed<< hist6Name<< " PMT = "<< k+1 <<"  Mean_Golden = "<<mean6[k]<<  " & " << "Mean_Current ="<<mean66[k]<<" Mean_Diff = "<<meanDiff6[k]<<endl;
          myfile << setprecision(2) << fixed <<"------"<< hist6Name << "\t"<< "PMT ="<<"\t"<<k+1<<"\t"<<mean6[k]<<"\t\t"<< mean66[k]<<"\t\t"<< meanDiff6[k]<<endl;//to store values in .txt file

          if (mean66[k]!=0.0) {

            if(fabs(meanDiff6[k])>2.5)//setting 2.5 mV as a cutoff point
              {
                if(first_error){
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<"Golden Run = "<<goldRunNum<<"     "<<"AND""     "<<"This Run = "<<run<<endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  cout<<" HERE THE MEAN DIFFERNCE OF PULSE PEDUSTAL BETWEEN GOLDEN RUN (MG) AND"<<endl;
                  cout<<"CURRENT RUN (M) IS GREATER THAN 2.5 mV FOR THE FOLLOWING HISTOGRAM(S)"<<endl;
                  cout<<"PLEASE CONSULT WITH BRAD SAWATZKY (brads@jlab.org) or ERIC POOSER(pooser@jlab.org)"<<endl;
                  cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Attention Needed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                  cout<<"___________________________________________________________________________________"<<endl;
                  first_error=false;
                }
                cout << setprecision(2) << fixed<<hist6Name<<"   "<<"PMT :"<<k+1<<"   "<<"MG ="<<"  "<<mean6[k] <<"  " 
                     <<"M ="<<"  "<< mean66[k] <<" " <<"MD"<<" "<< meanDiff6[k] << endl;//MG=mean golden,M = mean current, MD = mean diff
                cout<<"___________________________________________________________________________________"<<endl;
              }
          }
        }
    }
    

  myfile <<"************************************************************************************************"<<endl;
  myfile.close();
}
