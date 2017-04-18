////////////////////////////////////////////////////////////////////////////
// Impl�mentation de la classe TextJump 
//
// Classe permettant de stocker du texte et des donn�es d'URL dans une seule classe
//
// Ceci fait partie des exemples SDK d'Internet Information Server
// Copyright (C) 1997 Microsoft Corporation
// Tous droits r�serv�s.
//
// Ce code source est un compl�ment du mat�riel de r�f�rence Software 
// Development Kit Reference et de la documentation �lectronique connexe.
//
////////////////////////////////////////////////////////////////////////////

class TextJump 
{
	private String	m_strURL;
	private String	m_strText;

	TextJump(String strURL,String strText)
	{
		m_strURL = strURL;
		m_strText = strText;
	}

	public String getString()	{ return m_strText; }
	public String getURL()		{ return m_strURL; }
}

