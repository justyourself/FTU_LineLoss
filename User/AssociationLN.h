#define ADD_NUMBER 5        		 //插入LIST..
#define ADD1_NUMBER1    (ADD_NUMBER+1)        //插入list..
#define ADD_IMAGENUMBER 5//6        //增加LIST..
short CreatObjectListElement( short COM_No, unsigned char* Dest, unsigned short ClassId, unsigned char* LogicalName );
//short GetFirstObjectListDataGrade0( short COM_No, unsigned char* Dest );                      
short CreatSubObjectListHead( short COM_No, unsigned char* Dest );                            
short GetObjectListDataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );       
short GetObjectListRAMDataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
short GetObjectListRealRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );    
short GetObjectListECRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );      
short GetObjectListCUMMDRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );   
short GetObjectListMDExtRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );   
short GetAssociation_LNClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 

short GetNextAssociation_LNClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
unsigned char SetAssociation_LNClassData( short COM_No, unsigned char *Source , unsigned short DataLen );
                                                                                                     
short GetObjectListRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );                   
short GetObjectListProfile_genericGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );       
short GetObjectListDemand_registerGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );       
short GetObjectListRegister_monitorGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );      
short GetObjectListSingle_actionGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );         
short GetObjectListSpecial_days_tableGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );

//sky2013..
short GetObjectListPort_dataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );
short GetObjectListGPRSPort_dataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );
short GetObjectListRegister_ActivationGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//sky2013..
short GetObjectListImage_transfer_dataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );
short GetObjectListOther_transfer_dataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );
short GetObjectListOther_transfer_dataGrade1( short COM_No, unsigned char* Dest, short Satrt, short End );

short GetObjectListHRRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//sky2014..
//unsigned char ActionAssociation_LNClassData( unsigned char *Source , unsigned short DataLen );
unsigned char ActionAssociation_LNClassData( short COM_No,unsigned char *Source , unsigned short DataLen );

//short GetObjectListALNGrade0( short COM_No, unsigned char* Dest, short Satrt, short End );
short GetNextAssociation_LN1ClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short GetObjectListPublicAssociation( short COM_No, unsigned char* Dest, short Satrt, short End ) ;
short GetObjectListReadingAssociation( short COM_No, unsigned char* Dest, short Satrt, short End ) ;
short GetObjectListManagerAssociation( short COM_No, unsigned char* Dest, short Satrt, short End ) ;
short GetNextAssociation_LN2ClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );


