//////////////////////////////////////////////////////////////////////////////////
// [ Filterlist_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Filterlist.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ init ]
//////////////////////////////////////////////////////////////////////////////////
int C_Filterlist::init(const char* pTxtFile){
    
   if(!pTxtFile) return(C_FLIST_ERROR);
   
   bFilterlist = false;

   sFilterlist = pTxtFile;
   
   return(C_FLIST_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ read ] Sichern !!!
//////////////////////////////////////////////////////////////////////////////////
int C_Filterlist::read(){

   CDA_FilterList.clear();

   fFilterlist.open(sFilterlist.data(), ios_base::in | ios_base::out | ios_base::app);
   if(fFilterlist.is_open()) bFilterlist = true;
   else return(C_FLIST_ERROR);
   
   char zeile[300];
   memset(zeile, 0, sizeof(zeile));

   while(fFilterlist.getline(zeile, 300)){
      if(strlen(zeile)){
         C_Array<char>* pData = CDA_FilterList.addItem(strlen(zeile) + 1);
         if(!pData) return(C_FLIST_ERROR);
         memcpy(pData->getpBuffer(), zeile, strlen(zeile) + 1);
      }
   }
   
   fFilterlist.close();
   bFilterlist = false;
   
   return(C_FLIST_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ add ]
//////////////////////////////////////////////////////////////////////////////////
int C_Filterlist::add(const char* pSite){
    
   if(!pSite) return(C_FLIST_ERROR);
   
   int cSite = 0, cPoint = 0, pPoint[10];

   if(!(cSite = strlen(pSite))) return(C_FLIST_ERROR);

   memset(pPoint, 0, sizeof(pPoint));

   for(long n = cSite; n > 0; n--)
      switch(pSite[n]){
         case 46: pPoint[cPoint++] = n; break;
      }

   char test[300];
   memset(test, 0, 300);

   if(cPoint > 1) memcpy(test, &pSite[pPoint[1]] + 1, cSite - pPoint[1] - 1);
   else
   if(cPoint > 0) memcpy(test, &pSite, cSite);

   ////////////////////////////////////////////
   // Test ob schon vorhanden
   bool bLegal = true;

   for(int nItem = 0; nItem < CDA_FilterList.getnItems(); nItem++){
      C_Array<char>* pData = CDA_FilterList.getpItempData(nItem);
      if(!pData) return(C_FLIST_ERROR);
      if(!strcmp(pData->getpBuffer(), test)){
         bLegal = false;
         break;
      }
   }

   ////////////////////////////////////////////////////////////////////
   // Speichert neuen Eintrag in CDA_FilterList und fFilterlist
   if(bLegal){
      C_Array<char>* pData = CDA_FilterList.addItem(strlen(test) + 1); // + ZERO
      if(!pData) return(C_FLIST_ERROR);
      memcpy(pData->getpBuffer(), test, strlen(test) + 1);             // + ZERO

      fFilterlist.open(sFilterlist.data(), ios_base::in | ios_base::out | ios_base::trunc);
      if(fFilterlist.is_open()) bFilterlist = true;
      else return(C_FLIST_ERROR);
      
      for(int nItem = 0; nItem < CDA_FilterList.getnItems(); nItem++){
         C_Array<char>* pData = CDA_FilterList.getpItempData(nItem);
         if(!pData) return(C_FLIST_ERROR);
         fFilterlist.write(pData->getpBuffer(), pData->getcBuffer() - 1); // - ZERO
         fFilterlist << endl;
      }

      fFilterlist.close();
      bFilterlist = false;
   }

   return(C_FLIST_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ del ]
//////////////////////////////////////////////////////////////////////////////////
int C_Filterlist::del(const char* pSite){

   if(!pSite) return(C_FLIST_ERROR);
   
   int cSite = 0;

   if(!(cSite = strlen(pSite))) return(C_FLIST_ERROR);

   ////////////////////////////////////////////
   // Test ob vorhanden
   bool bLegal = true;
   int nItem = 0;
   for(nItem = 0; nItem < CDA_FilterList.getnItems(); nItem++){
      C_Array<char>* pData = CDA_FilterList.getpItempData(nItem);
      if(!pData) return(C_FLIST_ERROR);
      if(!strcmp(pData->getpBuffer(), pSite)){
         bLegal = false;
         break;
      }
   }
   
   ////////////////////////////////////////////////////////////////////
   // Loescht Eintrag in CDA_FilterList und fFilterlist
   if(!bLegal){
      CDA_FilterList.delItem(nItem);

      fFilterlist.open(sFilterlist.data(), ios_base::in | ios_base::out | ios_base::trunc);
      if(fFilterlist.is_open()) bFilterlist = true;
      else return(C_FLIST_ERROR);
      
      for(int nItem = 0; nItem < CDA_FilterList.getnItems(); nItem++){
         C_Array<char>* pData = CDA_FilterList.getpItempData(nItem);
         if(!pData) return(C_FLIST_ERROR);
         fFilterlist.write(pData->getpBuffer(), pData->getcBuffer() - 1); // - ZERO
         fFilterlist << endl;
      }

      fFilterlist.close();
      bFilterlist = false;
   }

   return(C_FLIST_READY);
}
