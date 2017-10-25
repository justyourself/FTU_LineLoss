//#include"Dat_Types.h"
//C0 01 81 00 07 00 00 63 01 00 FF 02 01
//SubsBlockPara
#define MAC_Get_Set_APDU									0
#define MAC_Get_Set_Tag										1
#define MAC_Get_Set_InvokeId							2
#define MAC_Get_Set_ClassId								3
#define MAC_Get_Set_Logical_name					5
#define MAC_Get_Set_Attribute							11
#define MAC_Get_Set_Selection_Access_flag		12
#define MAC_Get_Selection_Access_selector		13
#define MAC_Get_Profile_Para							14		//保存GET曲线参数
#define MAC_Set_First_Last_Block					13
#define MAC_Set_First_Block_Number				14
#define MAC_Set_First_Block_Data					18

                                        		
#define MAC_Set_Last_Block								3
#define MAC_Set_Block_Number							4
#define MAC_Set_Block_Data								8

//ACTIONG DATABLOCK..
#define MAC_Action_First_Last_Block					12
#define MAC_Action_First_Block_Number				13
#define MAC_Action_First_Block_Data					17

                                        		
#define MAC_Action_Last_Block								3
#define MAC_Action_Block_Number							4
#define MAC_Action_Block_Data								8

//GET-Request 		::= CHOICE
//{
#define get_request_normal    				1		// IMPLICIT Get-Request-Normal,
#define get_request_next      				2 		//IMPLICIT Get-Request-Next,
#define get_request_with_list   			3 		//IMPLICIT Get-Request-With-List
//}

//Get-Request-Normal 		::= SEQUENCE
//{
//	invoke-id-and-priority I        	Invoke-Id-And-Priority,
//	cosem-attribute-descriptor     	Cosem-Attribute-Descriptor,
//	access-selection-parameters  		Selective-Access-Descriptor OPTIONAL
//}

//Get-Request-Next 		::= SEQUENCE
//{
//	invoke-id-and-priority          Invoke-Id-And-Priority,
//	block-number                	Unsigned32
//}

//Get-Request-With-List 	::= SEQUENCE
//{
//	invoke-id-and-priority         	Invoke-Id-And-Priority,
//	attribute-descriptor-list        	SEQUENCE OF Cosem-Attribute-Descriptor-With-Selection
//}

//GET-Response 			::= CHOICE
//{
#define get_response_normal       			1 		//IMPLICIT Get-Response-Normal,
#define get_response_with_datablock 		2 		//IMPLICIT Get-Response-With-Datablock,
#define get_response_with_list      		3 		//IMPLICIT Get-Response-With-List
//}

//Get-Response-Normal 	::= SEQUENCE
//{
//	invoke-id-and-priority      	Invoke-Id-And-Priority,
//	result                   	Get-Data-Result
//}

//Get-Response-With-Datablock 	 ::= SEQUENCE
//{
//	invoke-id-and-priority      Invoke-Id-And-Priority,
//	result                   	DataBlock-G
//}

//DataBlock-G ::= SEQUENCE    -- G == GET.response服务使用的DataBlock
//{
//last-block 			BOOLEAN,
//block-number 		Unsigned32,
//result 				CHOICE 
//		{
//			raw-data 				[0] IMPLICIT OCTETSTRING,
//			data-access-result 		[1] IMPLICIT Data-Access-Result
//		}
//}

//Get-Response-With-List		 ::= SEQUENCE
//{
//	invoke-id-and-priority     	Invoke-Id-And-Priority,
//	result                   	SEQUENCE OF Get-Data-Result
//}

//SET-Request 		::= CHOICE
//{
#define set_request_normal                 	 			1 		//IMPLICIT Set-Request-Normal,
#define set_request_with_first_datablock        		2 		//IMPLICIT Set-Request-With-First-Datablock,
#define set_request_with_datablock            			3 		//IMPLICIT Set-Request-With-Datablock,
#define set_request_with_list                	 		4 		//IMPLICIT Set-Request-With-List,
#define set_request_with_list_and_first_datablock  		5 		//IMPLICIT Set-Request-With-List-And-First-Datablock
//}

//Set-Request-Normal 		::= SEQUENCE
//{
//	invoke-id-and-priority      	Invoke-Id-And-Priority,
//	cosem-attribute-descriptor  	Cosem-Attribute-Descriptor,
//	access-selection          	Selective-Access-Descriptor OPTIONAL,
//	value                   	Data
//}

//Set-Request-With-First-Datablock 	::= SEQUENCE
//{
//	invoke-id-and-priority      Invoke-Id-And-Priority,
//	cosem-attribute-descriptor  	Cosem-Attribute-Descriptor,
//	access-selection          	Selective-Access-Descriptor OPTIONAL,
//	datablock                DataBlock-SA
//} 

//Set-Request-With-Datablock 		::= SEQUENCE
//{
//	invoke-id-and-priority      Invoke-Id-And-Priority,
//	datablock                	DataBlock-SA
//}

//Set-Request-With-List 		::= SEQUENCE
//{
//	invoke-id-and-priority     	Invoke-Id-And-Priority,
//	attribute-descriptor-list     	SEQUENCE OF Cosem-Attribute-Descriptor-With-Selection,
//	value-list                	SEQUENCE OF Data
//}

//Set-Request-With-List-And-With-First-Datablock 	::= SEQUENCE
//{
//	invoke-id-and-priority      	Invoke-Id-And-Priority,
//	attribute-descriptor-list    	SEQUENCE OF Cosem-Attribute-Descriptor-With-Selection,
//	datablock               	DataBlock-SA
//}
//SET-Response 		::= CHOICE
//{
		#define set_response_normal              				1	 	//IMPLICIT Set-Response-Normal,
		#define set_response_datablock             			2	 	//IMPLICIT Set-Response-Datablock,
		#define set_response_last_datablock         		3	 	//IMPLICIT Set-Response-Last-Datablock,
		#define set_response_last_datablock_with_list  	4	 	//IMPLICIT Set-Response-Last-Datablock-With-List,
		#define set_response_with_list              		5	 	//IMPLICIT Set-Response-With-List
//}

//Set-Response-Normal 	::= SEQUENCE
//{
//	invoke-id-and-priority   	Invoke-Id-And-Priority,
//	result                	Data-Access-Result
//} 

//Set-Response-Datablock 		::= SEQUENCE
//{
//	invoke-id-and-priority    	Invoke-Id-And-Priority,
//	block-number          	Unsigned32
//}

//Set-Response-Last-Datablock 	::= SEQUENCE
//{
//	invoke-id-and-priority  		Invoke-Id-And-Priority,
//	result                	Data-Access-Result,
//	block-number         	Unsigned32
//} 

//Set-Response-Last-Datablock-With-List 		::= SEQUENCE
//{
//	invoke-id-and-priority  		Invoke-Id-And-Priority,
//	result                	SEQUENCE OF Data-Access-Result,
//	block-number        		Unsigned32
//}
//Set-Response-With-List 		::= SEQUENCE
//{
//	invoke-id-and-priority  		Invoke-Id-And-Priority,
//	result                	SEQUENCE OF Data-Access-Result
//}

//Action-Response-With-Optional-Data :: = SEQUENCE
//{
//	result Action-Result,
//	return-parameters Get-Data-Result OPTIONAL
//}

//Action-Result ::= ENUMERATED
//{
//	success (0),
//	hardware-fault (1),
//	temporary-failure (2),
//	read-write-denied (3),
//	object-undefined (4),
//	object-class-inconsistent (9),
//	object-unavailable (11),
//	type-unmatched (12),
//	scope-of-access-violated (13),
//	data-block-unavailable (14),
//	long-action-aborted (15),
//	no-long-action-in-progress (16),
//	other-reason (250)
//}

//ACTION-Request ::= CHOICE                                                                                    
//{                                                                                                            
		#define	action_request_normal 						1 	//IMPLICIT Action-Request-Normal,                                                    
		#define	action_request_next_pblock 				2 	//IMPLICIT Action-Request-Next-Pblock,                                          
		#define	action_request_with_list 					3 	//IMPLICIT Action-Request-With-List,                                              
		#define	action_request_with_first_pblock 	4 	//IMPLICIT Action-Request-With-First-Pblock,                              
		#define	action_request_with_list_and_first_pblock 	5 	//IMPLICIT Action-Request-With-List-And-First-Pblock,            
		#define	action_request_with_pblock 				6 	//IMPLICIT Action-Request-With-Pblock                                           
//}                                                                                                            
//Action-Request-Normal ::= SEQUENCE                                                                           
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	cosem-method-descriptor Cosem-Method-Descriptor,                                                             
//	method-invocation-parameters Data OPTIONAL                                                                   
//}                                                                                                            
//Action-Request-Next-Pblock ::= SEQUENCE                                                                      
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	block-number Unsigned32                                                                                      
//}                                                                                                            
//Action-Request-With-List ::= SEQUENCE                                                                        
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	cosem-method-descriptor-list SEQUENCE OF Cosem-Method-Descriptor,                                            
//	method-invocation-parameters SEQUENCE OF Data28                                                              
//}                                                                                                            
//Action-Request-With-First-Pblock ::= SEQUENCE                                                                
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	cosem-method-descriptor Cosem-Method-Descriptor,                                                             
//	pblock DataBlock-SA                                                                                          
//}                                                                                                            
//Action-Request-With-List-And-With-First-Pblock ::= SEQUENCE                                                  
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	cosem-method-descriptor-list SEQUENCE OF Cosem-Method-Descriptor,                                            
//	pblock DataBlock-SA                                                                                          
//}                                                                                                            
//Action-Request-With-Pblock ::= SEQUENCE                                                                      
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	pBlock DataBlock-SA                                                                                          
//}                                                                                                            
//ACTION-Response ::= CHOICE                                                                                   
//{                                                                                                            
		#define	action_response_normal 						  1 	//IMPLICIT Action-Response-Normal,                                                  
		#define	action_response_with_pblock 				2 	//IMPLICIT Action-Response-With-Pblock,                                        
		#define	action_response_with_list					  3 	//IMPLICIT Action-Response-With-List,                                            
		#define	action_response_next_pblock 				4 	//IMPLICIT Action-Response-Next-Pblock                                         
//}                                                                                                            
//Action-Response-Normal ::= SEQUENCE                                                                          
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	single-response Action-Response-With-Optional-Data                                                           
//}                                                                                                            
//Action-Response-With-Pblock ::= SEQUENCE                                                                     
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	pblock DataBlock-SA                                                                                          
//}                                                                                                            
//Action-Response-With-List ::= SEQUENCE                                                                       
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	list-of-responses SEQUENCE OF Action-Response-With-Optional-Data                                             
//}                                                                                                            
//Action-Response-Next-Pblock ::= SEQUENCE                                                                     
//{                                                                                                            
//	invoke-id-and-priority Invoke-Id-And-Priority,                                                               
//	block-number Unsigned32                                                                                      
//}                                                                                                            



//Integer8             	::= INTEGER(-128..127)
//Integer16            	::= INTEGER(-32 768..32 767)
//Integer32            	::= INTEGER(-2 147 483 648..2 147 483 647)
//Integer64            	::= INTEGER(-263.. 263-1)
//Unsigned8           	::= INTEGER(0..255)
//Unsigned16          	::= INTEGER(0..65 535)
//Unsigned32          	::= INTEGER(0..4 294 967 295)
//Unsigned64          	::= INTEGER(0..264-1)
//Invoke-Id-And-Priority   	:= BIT STRING (SIZE(8))
//{
//invoke-id (0…6),
//priority (7)
//}
//ObjectName                 	::= Integer16
//Cosem-Class-Id             	::= Unsigned16
//Cosem-Object-Instance-Id   	::= OCTET STRING (SIZE(6))
//Cosem-Object-Attribute-Id  	::= Integer8
//Cosem-Object-Method-Id     	::= Integer8
//Cosem-Attribute-Descriptor 	::= SEQUENCE
//{
//class-id      				Cosem-Class-Id,
//instance-id   				Cosem-Object-Instance-Id,
//attribute-id  				Cosem-Object-Attribute-Id
//}
//Cosem-Method-Descriptor ::= SEQUENCE
//{
//class-id     					Cosem-Class-Id,
//instance-id  					Cosem-Object-Instance-Id,
//method-id    					Cosem-Object-Method-Id
//}
//Selective-Access-Descriptor ::= SEQUENCE
//{
//access-selector   			Unsigned8,
//access-parameters 			Data
//}
//Cosem-Attribute-Descriptor-With-Selection ::= SEQUENCE
//{
//cosem-attribute-descriptor  	Cosem-Attribute-Descriptor
//access-selection           	Selective-Access-Descriptor OPTIONAL
//}
//Get-Data-Result ::= CHOICE
//{
		#define Get_data               			0 		//Data,
		#define Get_data_access_result 			1 		//IMPLICIT Data-Access-Result
//}
//Action-Response-With-Optional_Data :: = SEQUENCE
//{
//result             			Action-Result,
//return-parameters  			Get-Data-Result OPTIONAL
//}

//DataBlock-G ::= SEQUENCE    -- G == GET.response服务使用的DataBlock
//{
//last-block 			BOOLEAN,
//block-number 		Unsigned32,
//result 				CHOICE {
//raw-data 				[0] IMPLICIT OCTETSTRING,
//data-access-result 		[1] IMPLICIT Data-Access-Result
//}
//}
//DataBlock-SA ::= SEQUENCE  -- SA == SET.request和ACTION.request服务使用的DataBlock
//{
//last-block 			BOOLEAN,
//block-number 		Unsigned32,
//raw-data 				OCTETSTRING
//}


//Data-Access-Result ::= ENUMERATED
//{
#define success 										0
#define hardware_fault 							1		//硬件原因使访问失败
#define temporary_failure 					2
#define read_write_denied 					3
#define object_undefined 						4		//对象在VDE上未定义
#define object_class_inconsistent 	9		//对象类与要求的服务不兼容
#define object_unavailable 					11		//VDE控制不可及的对象
#define type_unmatched 							12
#define scope_of_access_violated 		13		//由于权限因数使访问遭拒绝
#define data_block_unavailable 			14
#define long_get_aborted 						15
#define no_long_get_in_progress 		16
#define long_set_aborted 						17
#define no_long_set_in_progress 		18
#define other_reason 								250
//}

//Data 	::= CHOICE
//{
#define null_data                		0 		//IMPLICIT NULL,
#define Data_array                   	1 		//IMPLICIT SEQUENCE OF Data,
#define Data_structure                  2 		//IMPLICIT SEQUENCE OF Data,
#define Data_boolean                	3	 		//IMPLICIT BOOLEAN,
#define Data_bit_string                 4 		//IMPLICIT BIT STRING,
#define Data_double_long             	5 		//IMPLICIT Integer32,
#define Data_double_long_unsigned    	6 		//IMPLICIT Unsigned32,
#define Data_floating_point             7 		//IMPLICIT OCTET STRING(SIZE(4)) 
#define Data_octet_string            	9 		//IMPLICIT OCTET STRING,
#define Data_visible_string             10 		//IMPLICIT VisibleString,
#define Data_time                  		11 		//IMPLICIT GeneralizedTime,
#define Data_UTF8_string                12
#define Data_bcd                   		13 		//IMPLICIT Integer8,
#define Data_integer                	15 		//IMPLICIT Integer8,
#define Data_long                  		16 		//IMPLICIT Integer16,
#define Data_unsigned              		17 		//IMPLICIT Unsigned8,
#define Data_long_unsigned         		18 		//IMPLICIT Unsigned16,
#define Data_compact_array      		19 		//IMPLICIT SEQUENCE
//		{                                       
#define		contents_description  		0 		//TypeDescription,
#define		array_contents       		1 		//IMPLICIT OCTET STRING
//		}                                       
#define Data_long64                		20 		//IMPLICIT Integer64,
#define Data_long64_unsigned       		21 		//IMPLICIT Unsigned64,
#define Data_enum                 		22 		//IMPLICIT ENUMERATED,
#define Data_float32                	23 		//IMPLICIT OCTET STRING (SIZE(4)),
#define Data_float64                	24 		//IMPLICIT OCTET STRING (SIZE(8)),
#define Data_DateTime                	25
#define Data_Date                	    26
#define Data_Time                	    27


#define Data_do_not_care             	255 	//IMPLICIT NULL
//}

#define D_BOOLEAN	                		    3	 		//IMPLICIT BOOLEAN,
#define D_bit_string	             		    4	 		//D_bit_string..
#define D_Integer32             			    5 		//IMPLICIT Integer32,
#define D_Unsigned32		    				6 		//IMPLICIT Unsigned32,
#define D_Float 		          				7 		//IMPLICIT OCTET STRING(SIZE(4)) 
#define D_OctetStr  	 	         			9 		//IMPLICIT OCTET STRING,
#define D_VisibleStr    	 	      		    10 		//IMPLICIT VisibleString,
                                    	
#define D_Integer8                		        15 		//IMPLICIT Integer8,
#define D_Integer16                 	        16 		//IMPLICIT Integer16,
#define D_Unsigned8              			    17 		//IMPLICIT Unsigned8,
#define D_Unsigned16         					18 		//IMPLICIT Unsigned16,

#define D_ENUM 	                			    22 		//IMPLICIT ENUMERATED,

#define Class_id_Data							1
#define Class_id_Register						3
#define Class_id_Extended_register			    4
#define Class_id_Demand_register				5
#define Class_id_Register_activation		    6
#define Class_id_Profile_generic				7
#define Class_id_Clock							8
#define Class_id_Script_table					9
#define Class_id_Schedule						10
#define Class_id_Special_days_table			    11
#define Class_id_Association_SN					12
#define Class_id_Association_LN					15
#define Class_id_SAP_assignment					17
#define Class_id_Image_transfer					18
#define Class_id_IEC_local_port_setup		    19
#define Class_id_Activity_calendar			    20
#define Class_id_Register_monitor				21
#define Class_id_Single_action_schedule	        22
#define Class_id_IEC_HDLC_setup					23

#define Class_id_Utility_tables					26
#define Class_id_Auto_answer_setup		        28   //Auto answer (class_id = 28, version = 2)
#define Class_id_Auto_connect_setup			    29   //Auto connect (class_id = 29, version = 2)

#define Class_id_Push_Setup                     40
#define Class_id_TCP_UDP_setup					41
#define Class_id_IPv4_setup	                    42
//--42类是本地..
#define Class_id_MACaddr_GPRS	                43
#define Class_id_GPRS_modem_setup				45
#define Class_id_SMTP_setup						46
#define Class_id_Register_table					61
#define Class_id_Status_mapping					63
#define Class_id_Security_setup					64
#define Class_id_Disconnect_control			    70
#define Class_id_Limiter						71

//128--工厂自定义类
#define  Class_id_Manufacture_setup             128



//属性访问权限
#define no_access							0
#define read_only							1
#define write_only							2
#define read_and_write						3
#define authenticated_read_only 			4 
#define authenticated_write_only			5
#define authenticated_read_and_write	    6

//代码    	//单位      		量              单位        	 SI定义
#define Unit_a   	1	        //a      	 	// 	时间             	年
#define Unit_mo  	2	       	//mo          	//	时间              	月
#define Unit_wk  	3	       	//wk       	   	//	时间              	周              7*24*60*60s
#define Unit_d   	4	       	//d            	//	时间              	日              24*60*60 s
#define Unit_h   	5	       	//h        	   	//	时间             	小时            60 *60 s
#define Unit_min	6	       	//min.     	   	//	时间              	分              60 s
#define Unit_s   	7	     	    //s       	   	//	时间(t)            	秒              s
#define Unit_deg	8	 	   	    //°          	//	(相)角             	度              rad*180/π
#define Unit_Cel	9	   	   	  //0C         	//	温度(Θ)          	摄氏度          K-273.15
#define Unit_m3_h	15	   	    //(15)	       m3/h  		//	流量                                m3 /（60*60s）
#define Unit_W   	27	       	//W          	//	有功功率 （P）     	瓦              W = J/s
#define Unit_VA  	28	    	  //VA         	//	视在功率（S）   	
#define Unit_var 	29	  		  //var         	//	无功功率（Q）
#define Unit_Wh  	30	        //Wh         	//	有功能量                            W*(60*60s) 
#define Unit_VAh 	31	        //VAh         	//	视在能量                            VA*(60*60s)
#define Unit_varh	32	        //varh      	//	无功能量                            var*(60*60s)
#define Unit_A   	33	        //A         	//	电流（I）           安培            A
#define Unit_V   	35	        //V         	//	电压（U）          	伏特            V 
#define Unit_Hz  	44	        //Hz        	//	频率               	赫兹            1 /s
#define Unit_Rac 	45         	//Rac         	//	有功能量表常数                      1/（Wh）
#define Unit_Rre 	46    	    //Rre         	//	无功能量表常数
#define Unit_Rap 	47         	//Rap       	//	视在能量表常数
#define Unit_V2_h	48         	//V2/h        	// 	伏特平方小时                   		V2(60*60s)
#define Unit_A2_h	49         	//A2/h   		// 	安培平方小时                   		A2(60*60s)
#define Unit_FF		255    		  //计数        	//	无单位，缺单位，计数
#define Unit_Per   	56	       		//%

short CreatGetRequestNormalError( unsigned char* Dest, unsigned char InvokeId, unsigned char ErrorByte );
short JudgeClassPara( short COM_No, unsigned char* Dest, unsigned char* Source );
short SETProcess( unsigned char COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short GETProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short GetDataClassData( short COM_No,unsigned char* Dest, unsigned char* Source );
short GetRegisterECTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute );
short GetRegisterCUMDTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute );
short GetRegisterRealTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute );
short GetRegisterRegTab( unsigned char* Dest, unsigned char* Logical_Name, unsigned char Attribute );
short GetRegisterClassData( unsigned char* Dest, unsigned char* Source ); 
short GetExtendedRegisterClassData( unsigned char* Dest, unsigned char* Source );
short GetClockClassData( unsigned char* Dest, unsigned char* Source ); 
short GetSAP_assignmentClassData( unsigned char* Dest, unsigned char* Source ); 
short GetIECLocalPortSetupClassData( unsigned char* Dest, unsigned char* Source ); 
short GetIECHDLCSetupClassData( unsigned char* Dest, unsigned char* Source ); 
//short CreatObjectListElement( short COM_No, unsigned char* Dest, unsigned short ClassId, unsigned char* LogicalName );
void FillLongToBuff( unsigned char* Dest, unsigned long Data );
void FillIntegerToBuff( unsigned char* Dest, unsigned short Data );
unsigned short GetBuffToInteger( unsigned char* Data );
unsigned long GetBuffToLong( unsigned char* Data );
//short GetFirstObjectListDataGrade0( short COM_No, unsigned char* Dest ); 
//short CreatSubObjectListHead( short COM_No, unsigned char* Dest ); 
//short GetObjectListDataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetObjectListRealRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetObjectListECRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetObjectListCUMMDRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetObjectListMDExtRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetAssociation_LNClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 
short GetRequestNormalProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short JudgeRequestNext( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
//short GetNextAssociation_LNClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short GetRequestNextProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );

short CreatSetRequestNormalError( unsigned char* Dest, unsigned char InvokeId, unsigned char ErrorByte );
short JudgeSetClassPara( short COM_No, unsigned char* Dest, unsigned char* Source );
short SetRequestNormalProcess( unsigned char COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
unsigned char SetDataClassData( unsigned char COM_No,unsigned char *Source , unsigned short DataLen );
unsigned char SetRegisterClassData( unsigned char *Source , unsigned short DataLen );
unsigned char SetSAP_assignmentClassData( unsigned char *Source , unsigned short DataLen );
unsigned char SetClockClassData( unsigned char *Source , unsigned short DataLen );
unsigned char SetIECLocalPortSetupClassData( unsigned char *Source , unsigned short DataLen );
unsigned char SetIECHDLCSetupClassData( unsigned char *Source , unsigned short DataLen );

unsigned short GetDataClassTabItemNo( unsigned char* Logical_Name );
unsigned short GetMDTabItemNo( unsigned char* Logical_Name );
unsigned short GetECTabItemNo( unsigned char* Logical_Name );
unsigned short GetCUMDTabItemNo( unsigned char* Logical_Name );
unsigned short GetREALTabItemNo( unsigned char* Logical_Name );
unsigned short GetRegTabItemNo( unsigned char* Logical_Name );

short Get_with_datablock_Abnormity( unsigned char* Dest, unsigned char AbnormityType );
short SetRequestWithDataBlockProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff, short Type );

unsigned char* JudgeSetBlockData( unsigned char* Data, unsigned short Len, unsigned short* DataLen, unsigned char* ErrorByte );
unsigned char SetClassData( unsigned char COM_No, unsigned short ClassId, unsigned char* Data, unsigned short DataLen );

short CreatActionRequestNormalError( unsigned char* Dest, unsigned char InvokeId, unsigned char ErrorByte );
short JudgeActionClassPara( short COM_No, unsigned char* Dest, unsigned char* Source );
unsigned char ActionClassData( short COM_No, unsigned short ClassId, unsigned char* Data, unsigned short DataLen );
short ActionRequestNormalProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short ActionRequestWithDataBlockProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff, short Type ); //sky20140514..
short ACTIONProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );

//unsigned char SetAssociation_LNClassData( short COM_No, unsigned char *Source , unsigned short DataLen );
//
//short GetObjectListRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetObjectListProfile_genericGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetObjectListDemand_registerGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetObjectListRegister_monitorGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 
//short GetObjectListSingle_actionGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ); 

void EVENT_NOTIFICATIONProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );

unsigned short GetRAMDataClassTabItemNo( unsigned char* Logical_Name );
short GetNextAssociation_messageClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );

void 	LogToSytemtile(void);  //由逻辑设备名计算得到systemtitle;
