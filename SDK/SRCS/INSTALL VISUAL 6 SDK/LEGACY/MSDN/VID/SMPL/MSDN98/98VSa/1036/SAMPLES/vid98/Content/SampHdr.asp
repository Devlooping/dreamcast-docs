<%@ LANGUAGE=VBScript %>
<%
Select Case Request("SampID")
	Case 1
		Title="Entr�e des donn�es"
		Run="DEntry/Entry.asp"
		BTS="DEntryB.htm"
	Case 2
		Title="Citation du jour"
		Run="Quote/Quote.asp"
		BTS="QuoteB.htm"
	Case 3
		Title="Grille d�pendante"
		Run="Grid/Grid.asp"
		BTS="GridB.htm"
	Case 4
		Title="Requ�te personnalis�e"
		Run="Query/Query.asp"
		BTS="QueryB.htm"
	Case 5
		Title="Pr�f�rences de l'utilisateur"
		Run="Prefer/Prefer.asp"
		BTS="PrefB.htm"
	Case 6
		Title="Compteur d'acc�s"
		Run="Counter/Counter.asp"
		BTS="CounterB.htm"
	Case 7
		Title="Enqu�te"
		Run="Survey/Survey.asp"
		BTS="SurveyB.htm"
	Case 8
		Title="Utilisation des contr�les"
		Run="Controls/DTCTest.asp"
		BTS="ControlB.htm"
	Case 9
		Title="Chariot des courses"
		Run="ShopCart/ShopCart.htm"
		BTS="ShopCrtB.htm"
	Case 10
		Title="Notification de l'utilisateur"
		Run="UserNote/UserNote.asp"
		BTS="UserNtB.htm"
	Case 11
		Title="Notification par courrier �lectronique"
		Run="Email/Email.htm"
		BTS="EmailB.htm"
	Case 12
		Title="Livre d'or"
		Run="GuestBk/GuestBk.asp"
		BTS="GuestBkB.htm"
	Case 13
		Title="Transitions et filtres"
		Run="IE4/IE4.asp"
		BTS="IE4B.htm"
	Case 14
		Title="Applet VJ++"
		Run="Applet/Applet.asp"
		BTS="AppletB.htm"
	Case 15
		Title="Publicit� al�atoire"
		Run="RandomAd/RandomAd.asp"
		BTS="RandAdB.htm"
	Case 16
		Title="Utilisation des th�mes"
		Run="Themes/Themes.htm"
		BTS="ThemeB.htm"
	Case 17
		Title="Multim�dia"
		Run="MMedia/MMedia.htm"
		BTS="MMediaB.htm"
	Case 18
		Title="Chronologie"
		Run="TimeLine/TLFish.htm"
		BTS="TimeLnB.htm"
End Select
%>
<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Visual Studio 6.0">
</HEAD>
<body bgcolor="#FFFFFF" TOPMARGIN="56" LEFTMARGIN="40" LINK="FFFFFF" VLINK="#FFFFFF">
<font FACE="Arial, Helvetica, Verdana" SIZE="2">
<font SIZE="5">Exemple <%=Title%></font>
<br><br>
<table CELLSPACING="0" CELLPADDING="0" WIDTH="100%">
	<tr>
		<td BGCOLOR="#510000"><font FACE="Arial, Helvetica, Verdana" SIZE="1"><nobr>&nbsp;&nbsp;<a HREF="<%=Run%>" TARGET="SampMain"><b>Ex�cuter l'exemple</b></a>&nbsp;&nbsp;</nobr></td>
		<td BGCOLOR="#FFFFFF" WIDTH="2">&nbsp;</td>
		<td BGCOLOR="#FFFFFF" WIDTH="60%">&nbsp;</td>
		<td BGCOLOR="#FFFFFF" WIDTH="2">&nbsp;</td>
		<td BGCOLOR="#510000"><font FACE="Arial, Helvetica, Verdana" SIZE="1"><nobr>&nbsp;&nbsp;<a HREF="<%=BTS%>" TARGET="SampMain"><b>L'envers du d�cor</b></a>&nbsp;&nbsp;</nobr></td>
	</tr>
</table>
</BODY>
</HTML>
