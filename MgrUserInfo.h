// ---------------------------------------------------------------------------

#ifndef MgrUserInfoH
#define MgrUserInfoH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

// ---------------------------------------------------------------------------
class TUserInfo
{
private:
protected:

public:
	String ch_div_cd;
	String usr_id;
	String usr_nm;
	String token;
	String usr_ip;
	String mac_addr; // 컴퓨터 맥 어드래스
	String app_vsn; // 프로그램 버전
	String lang;
	String format;
	String os_type;
	String mphon_no;
	String inphon_no;
	String e_mail;
	String user_rprn_grp;

	__fastcall TUserInfo( )	{ } ;
	__fastcall TUserInfo( String ch_div_cd, String usr_id,
		String usr_nm, String token,
		String usr_ip, String MacAddr, String lang,
		String  format, String os_type,
		String mphon_no = String( ), String inphon_no = String( ),
		String e_mail = String( ), String user_rprn_grp = String( ),
		String app_vsn =  String( ) );
} ;
#endif
