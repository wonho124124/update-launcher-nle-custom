// ---------------------------------------------------------------------------
/****************************************************************************
 *  ���α׷� �� : MgrUserInfo
 *  ���α׷� ���� : ����� ������ �����ϱ� ���� ���� ( ��� �� ���� �Ҷ� �߰� - �ʼ�)
 *  ������        : ������
 *  �ۼ�����      :
 *
 *  �����̷�
 *  ----------------------------------------------------------
 *  �ۼ���   /    ����      /     ����
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
