#define PDU_initiate_Request						1
#define PDU_read_Request							5
#define PDU_write_Request						6
#define PDU_initiate_Response					8
#define PDU_read_Response						12
#define PDU_write_Response						13
#define PDU_confirmed_Service_Error				14
#define PDU_unconfirmed_Write_Request			22
#define PDU_information_Report_Request			24

//-- with global ciphering
#define PDU_GLO_initiate_Request					33
#define PDU_GLO_read_Request						37
#define PDU_GLO_write_Request					38
#define PDU_GLO_initiate_Response				40
#define PDU_GLO_read_Response					44
#define PDU_GLO_write_Response					45
#define PDU_GLO_confirmed_Service_Error			46
#define PDU_GLO_unconfirmed_Write_Request		54
#define PDU_GLO_information_Report_Request		56
//
#define APDU_AARQ								96
#define APDU_AARE								97
#define RLRQ_APDU								98
#define RLRE_APDU								99
#define APDU_GET_Request							192     //0xc0
#define APDU_SET_Request							193     //0xc1
#define APDU_EVENT_NOTIFICATION_Request			194     //0xc2
#define APDU_ACTION_Request 						195     //0xc3
#define APDU_GET_Response   						196 	//0xc4
#define APDU_SET_Response   						197		//0xc5
#define APDU_ACTION_Response						199

//带加密（全局密钥 + DK密钥）
#define GLO_GET_Request							200     //0xc8
#define GLO_SET_Request							201     //0xc9
#define GLO_EVENT_NOTIFICATION_Request			202     //0xcA
#define GLO_ACTION_Request 						203     //0xcB
#define GLO_GET_Response   						204
#define GLO_SET_Response   						205
#define GLO_ACTION_Response						207

//DK密钥..
#define DED_GET_Request							208     //0xD0
#define DED_SET_Request							209     //0xD1
#define DED_EVENT_NOTIFICATION_Request			210     //0xD2
#define DED_ACTION_Request 						211     //0xD3
#define DED_GET_Response   						212
#define DED_SET_Response   						213
#define DED_ACTION_Response						215

//-- the exception respsonse pdu
#define APDU_EXCEPTION_response 				216 		//IMPLICIT OCTET STRING
//Acess
#define Acess_Request           217
#define Acess_Response          218
//general APDUS
#define General_GLO_CIP		    219
#define General_DED_CIP 	    220
#define General_CIP     	    221
#define General_SIGNING 		223
#define General_Block_Transfer  224
//EXCEPTION-Response ::=SEQUENCE
//{
#define state_error 							0
#define service_error 						1
//}
//state_error IMPLICIT ENUMERATED
//{
#define service_not_allowed 				1
#define service_unknown 						2
//}
//service_error IMPLICIT ENUMERATED
//{
#define operation_not_possible 					1
#define service_not_supported 					2
#define service_error_other_reason 			3
//}

//AARQ-apdu    ::=  [APPLICATION 0] IMPLICIT SEQUENCE
//                                       -- [APPLICATION 0] == [ 60H ] = [ 96 ]
#define AARQ_protocol_version							0x80	//IMPLICIT BIT STRING {version1 (0)} DEFAULT {version1}
#define AARQ_application_context_name			0xA1
#define AARQ_called_AP_title							0xA2	//OPTIONAL
#define AARQ_called_AE_qualifier					0xA3	//OPTIONAL	
#define AARQ_called_AP_invocation_id			0xA4	//OPTIONAL
#define AARQ_called_AE_invocation_id			0xA5	//OPTIONAL
#define AARQ_calling_AP_title							0xA6	//OPTIONAL
#define AARQ_calling_AE_qualifier					0xA7	//OPTIONAL	
#define AARQ_calling_AP_invocation_id			0xA8	//OPTIONAL
#define AARQ_calling_AE_invocation_id			0xA9	//OPTIONAL
//-- 如果只使用内核，下面的字段不出现。 		
#define AARQ_sender_acse_requirements			0x8A	//IMPLICIT OPTIONAL
//-- 如果选用身份验证单元，下面的字段应出现
#define AARQ_mechanism_name								0x8B	//IMPLICIT OPTIONAL
//-- 如果选用身份验证单元，下面的字段应出现
#define AARQ_calling_authentication_value		0xAC	//EXPLICIT OPTIONAL
#define AARQ_implementation_information			0xBD	//IMPLICIT OPTIONAL
#define AARQ_user_information								0xBE	//IMPLICIT OPTIONAL
                                        	
//AARE-apdu     ::=  [APPLICATION 1]  IMPLICIT SEQUENCE
//                                         -- [APPLICATION 1] == [ 61H ] = [ 97 ]
#define AARE_protocol_version								0x80	//IMPLICIT BIT STRING {version1 (0)} DEFAULT {version1}
#define AARE_application_context_name				0xA1
#define AARE_result													0xA2
#define AARE_result_source_diagnostic				0xA3
#define AARE_calling_AP_title								0xA4	//OPTIONAL
#define AARE_calling_AE_qualifier						0xA5	//OPTIONAL	
#define AARE_calling_AP_invocation_id				0xA6	//OPTIONAL
#define AARE_calling_AE_invocation_id				0xA7	//OPTIONAL
//-- 如果只使用内核，下面的字段不出现。 	
#define AARE_responder_acse_requirements		0x88	//IMPLICIT OPTIONAL
//-- 如果选用身份验证单元，下面的字段应出现。
#define AARE_mechanism_name									0x89	//IMPLICIT OPTIONAL
//-- 如果选用身份验证单元，下面的字段应出现。
#define AARE_responding_authentication_value	0xAA	//EXPLICIT OPTIONAL
#define AARE_implementation_information				0xBD	//IMPLICIT OPTIONAL
#define AARE_user_information									0xBE	//IMPLICIT OPTIONAL

//Association-result        ::= INTEGER
#define Result_accepted_0									0x00
#define Result_rejected_permanent_1				0x01
#define Result_rejected_transient_2				0x02

//Associate-source-diagnostic ::= CHOICE
#define Diagnostic_acse_service_user			0xA1	
#define Diagnostic_acse_service_provider	0xA2	

//acse-service-user [1] INTEGER
#define Diagnostic_null_0															0	
#define Diagnostic_no_reason_given_1									1	
#define Diagnostic_application_context_name_not_supported_2				2	
#define Diagnostic_authentication_mechanism_name_not_recognised_11		11	
#define Diagnostic_authentication_mechanism_name_required_12			12
#define Diagnostic_authentication_failure_13							13
#define Diagnostic_authentication_required_14							14

//acse-service-provider [2] INTEGER
//#define Diagnostic_null_0												0	
#define Diagnostic_no_reason_given_1									1
#define Diagnostic_no_common_acse_version_2								2

#define APPL_OPEN_4_S2						1		
#define APPL_OPEN_5_S2						2		
#define APPL_OPEN_5_S3						3		
#define APPL_OPEN_7_S1						4		
#define APPL_OPEN_7_S2						5		
#define APPL_OPEN_7_S3						6		
#define APPL_OPEN_7_S4						7		
#define APPL_OPEN_7_S5						8		
#define APPL_OPEN_7_S6						9		
#define APPL_OPEN_7_S7						10		
#define APPL_OPEN_7_S8						11		
#define APPL_OPEN_8							  12		
#define APPL_OPEN_9							  13		
#define APPL_OPEN_12_S1						14
#define APPL_OPEN_13_S3						15
#define APPL_OPEN_14_S2						16

//ConfirmedServiceError ::= CHOICE
#define CSError_initiateError        		1 		//ServiceError
#define CSError_getStatus            		2 		//ServiceError
#define CSError_getNameList         		3 		//ServiceError
#define CSError_getVariableAttribute  	4 		//ServiceError
#define CSError_read                		5 		//ServiceError
#define CSError_write               		6 		//ServiceError
#define CSError_getDataSetAttribute   	7 		//ServiceError
#define CSError_getTIAttribute        	8 		//ServiceError
#define CSError_changeScope         		9 		//ServiceError
#define CSError_start                		10	 	//ServiceError
#define CSError_stop                		11	 	//ServiceError
#define CSError_resume              		12	 	//ServiceError
#define CSError_makeUsable          		13	 	//ServiceError
#define CSError_initiateLoad          	14	 	//ServiceError
#define CSError_loadSegment         		15	 	//ServiceError
#define CSError_terminateLoad        		16	 	//ServiceError
#define CSError_initiateUpLoad       		17	 	//ServiceError
#define CSError_upLoadSegment      			18	 	//ServiceError
#define CSError_terminateUpLoad       	19	 	//ServiceError

//ServiceError ::= CHOICE
#define SError_application_reference 		 0 				//IMPLICIT ENUMERATED 
//-- DLMS提供者                                             //
#define SError0_other                 				    0     //
#define SError0_time_elapsed                  		1 		//-- 请求已超时
#define SError0_application_unreachable        		2 		//-- 找不到对等的AEi
#define SError0_application_reference_invalid    	3 		//-- 访问出错
#define SError0_application_context_unsupported  	4 		//-- application-context不兼容
#define SError0_provider_communication_error   		5 		//-- 在本地或远方设备中发生错误
#define SError0_deciphering_error              		6 		//-- 检测到加密功能出错

#define SError_hardware_resource 			            1	 			//IMPLICIT ENUMERATED 
//-- VDE硬件故障
#define SError_other                         		  0
#define SError1_memory_unavailable             		1
#define SError1_processor_resource_unavailable 		2
#define SError1_mass_storage_unavailable       		3
#define SError1_other_resource_unavailable     		4

#define SError_vde_state_error 				            2 				//IMPLICIT ENUMERATED 
//-- 差错源说明
#define SError2_other              					0
#define SError2_no_dlms_context    					1
#define SError2_loading_data_set   					2
#define SError2_status_nochange    					3
#define SError2_status_inoperable  					4

#define SError_service 						3 				//IMPLICIT ENUMERATED 
//-- 服务处理出错
#define SError2_other               				0		
#define SError2_pdu_size            				1		//-- pdu太长
				                                	 		//-- (参见配套规范)
#define SError2_service_unsupported					2		//-- 参见一致性块的说明

#define SError_definition 					4 				//IMPLICIT ENUMERATED 
//-- 服务中绑定的对象出错
#define SError4_other                          		0		
#define SError4_object_undefined               		1		//-- VDE中没有定义该对象
#define SError4_object_class_inconsistent      		2		//-- 对象类与请求的服务不兼容
#define SError4_object_attribute_inconsistent  		3		//-- 对象属性不一致

#define SError_access 						5 				//IMPLICIT ENUMERATED 
//-- 对象访问出错
#define SError5_other                         		0
#define SError5_scope_of_access_violated      		1 		//-- 由于权限的原因，访问被否定
#define SError5_object_access_invalid         		2 		//-- 访问与对象属性不兼容
#define SError5_hardware_fault                		3 		//-- 由于硬件的原因，访问失败
#define SError5_object_unavailable            		4 		//-- 找不到VDE处理的对象

#define SError_initiate 					6 				//IMPLICIT ENUMERATED 
//-- 初始化服务出错
#define SError6_other                      			0	
#define SError6_dlms_version_too_low       			1	 	//-- 提请的DLMS版本太低
#define SError6_incompatible_conformance   			2	 	//-- 提请的服务不满足
#define SError6_pdu_size_too_short         			3	 	//-- 提请的pdu太短
#define SError6_refused_by_the_VDE_Handler   		4	 	//-- VAA 不能创建或不允许创建

#define SError_load_data_set 				7 				//IMPLICIT ENUMERATED 
//-- 数据集装入服务出错
#define SError7_other                         		0		
#define SError7_primitive_out_of_sequence        	1		//-- 按照 DataSet
                                       						//-- 装入状态变换
#define SError7_not_loadable                   		2		//-- 可装入属性被设为FALSE
#define SError7_dataset_size_too_large           	3		//-- Data Set太大
#define SError7_not_awaited_segment             	4		//-- 提请的段不处于等待状态
#define SError7_interpretation_failure           	5		//-- 段解释出错
#define SError7_storage_failure                  	6		//-- 段存储出错
#define SError7_data_set_not_ready               	7		//-- 上载时Data Set处于不正确的状态

#define SError_change_scope 				8 				//IMPLICIT 保留。

#define SError_task 						9	 			//IMPLICIT ENUMERATED 
//-- TI 服务出错
#define SError9_other             					0		
#define SError9_no_remote_control 					1		//-- 远方控制参数被设为FALSE
#define SError9_ti_stopped        					2		//-- TI处于停止状态
#define SError9_ti_running        					3		//-- TI处于运行状态
#define SError9_ti_unusable       					4		//-- TI处于不可用状态

#define SError_other10 						10 				//IMPLICIT ENUMERATED

#define Conformance_block_transfer_with_get         11
#define Conformance_block_transfer_with_set         12
#define Conformance_block_transfer_with_action      13
#define Conformance_information_report              15
#define Conformance_parameterized_access            18
#define Conformance_get                             19
#define Conformance_set                             20
#define Conformance_selective_access                21
#define Conformance_event_notification              22
#define Conformance_action                          23


unsigned short CreatExceptionResponseAPDU( unsigned char* Dest );
unsigned short APPLProcess( unsigned char COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
unsigned char GetSTRINGLowestBIT( unsigned char* Data, short Len );
void FillApplicationContextName( unsigned char* Dest );
short CompareApplicationContextName( unsigned char* Source );
short GetComformanceState( short COM_No, unsigned char* Source, unsigned char SecretState );
short	CreatUser_information( short COM_No, unsigned char* Dest, unsigned short ErrByte );
//short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length );
short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char SecretState );	
//short	ProcessAARQItem( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length );	
short	ProcessAARQItem( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char* SecretState );	
short GetAAREResult_Diagnostic(  short COM_No,unsigned char* Dest, unsigned short RejectType );
short AARQProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short RLRQProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );

void FillMechanism_name( unsigned char* Dest );
short CompareMechanism_name( unsigned char* Source );
//sky20140613..
short AARQHProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short CompareAuthentication_HLS( unsigned char* Source );

unsigned short GetRPublic_clientNO( unsigned char* OBIS_Name );
unsigned short GetRReading_clientNO( unsigned char* OBIS_Name );
unsigned short GetWPR_clientNO( unsigned char* OBIS_Name );
unsigned short GetAPR_clientNO( unsigned char* OBIS_Name );

unsigned short GetRPR_clientNO( unsigned char* OBIS_Name );
int Decrypt_OR_Encrypt( unsigned short COM_No,unsigned char* Ciphertext,unsigned short *nCtextLen,unsigned char use_crypt );

