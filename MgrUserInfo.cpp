// ---------------------------------------------------------------------------
/****************************************************************************
 *  프로그램 명 : MgrUserInfo
 *  프로그램 설명 : 사용자 정보를 전달하기 위해 생성 ( 상속 폼 생성 할때 추가 - 필수)
 *  개발자        : 김형기
 *  작성일자      :
 *
 *  수정이력
 *  ----------------------------------------------------------
 *  작성자   /    일자      /     내용
 *  -----------------------------------------------------------
 *************************************************************************** */

#pragma hdrstop

#include "MgrUserInfo.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TUserInfo::TUserInfo( String ch_div_cd, String usr_id, String usr_nm,
	String token, String usr_ip, String MacAddr,
	String lang, String format, String os_type,
	String mphon_no, String inphon_no, String e_mail, String user_rprn_grp, String app_vsn )
{
	this->ch_div_cd     = ch_div_cd;
	this->usr_id        = usr_id;
	this->usr_nm        = usr_nm;
	this->token         = token;
	this->usr_ip        = usr_ip;
	this->mac_addr      = MacAddr;
	this->lang          = lang;
	this->format        = format;
	this->os_type       = os_type;
	this->mphon_no      = mphon_no;
	this->inphon_no     = inphon_no;
	this->e_mail        = e_mail;
	this->user_rprn_grp = user_rprn_grp;
	this->app_vsn       = app_vsn;
}
