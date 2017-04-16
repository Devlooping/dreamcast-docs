Attribute VB_Name = "modSubMain"
'------------------------------------------------------------------------
'�Copyright � 1997 Microsoft Corporation. Tous droits r�serv�s.
' Vous disposez du droit non soumis � redevance d'utiliser, de modifier,
' de reproduire et de distribuer, de quelque mani�re que ce soit, les fichiers
' d'application exemple (et/ou toute version modifi�e) tant que
' vous reconnaissez que Microsoft ne pr�sente aucune garantie,
' obligation ou responsabilit� pour tout fichier d'application exemple.
'------------------------------------------------------------------------
Option Explicit

'------------------------------------------------------------
' Constantes d�clar�es...
'------------------------------------------------------------
Public Const IID_IDispatch = "{00020400-0000-0000-C000-000000000046}"           ' ID de l'interface IDispatch
Public Const IID_IPersistStorage = "{0000010A-0000-0000-C000-000000000046}"     ' ID de l'interface IPersistStorage
Public Const IID_IPersistStream = "{00000109-0000-0000-C000-000000000046}"      ' ID de l'interface IPersistStream
Public Const IID_IPersistPropertyBag = "{37D84F60-42CB-11CE-8135-00AA004BB851}" ' ID de l'interface IPersistPropertyBag

' Option de d�finitions de bit pour l'interface IObjectSafety
Public Const INTERFACESAFE_FOR_UNTRUSTED_CALLER = &H1   ' Appelant de l'interface n'est peut-�tre pas s�curis�
Public Const INTERFACESAFE_FOR_UNTRUSTED_DATA = &H2     ' Donn�es pass�es � l'interface ne sont peut-�tre pas s�curis�

Public Const E_NOINTERFACE = &H80004002 ' Ce genre d'interface n'est pas pris en charge
Public Const E_FAIL = &H80004005        ' Erreur inattendue

Public Const MAX_GUIDLEN = 40           ' La constante doit avoir une valeur �gale � 40 pour NT.

'------------------------------------------------------------
' API d�clar�es...
'------------------------------------------------------------
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (pDest As Any, pSource As Any, ByVal ByteLen As Long)
Public Declare Function StringFromGUID2 Lib "ole32.dll" (rguid As Any, ByVal lpstrClsId As Long, ByVal cbMax As Integer) As Long

'------------------------------------------------------------
' UDT d�clar�es...
'------------------------------------------------------------
Public Type uGUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(7) As Byte
End Type

'------------------------------------------------------------
Public Function GetIIDFromPTR(ByVal riid As Long) As String
'------------------------------------------------------------
    Dim Rc          As Long                                         ' Code de renvoie de fonction
    Dim rClsId      As uGUID                                        ' GUID de structure
    Dim bIID()      As Byte                                         ' Tableau d'octets pour ID de l'interface
'------------------------------------------------------------
    If (riid <> 0) Then                                             ' Valide le pointeur de l'ID de l'interface
        CopyMemory rClsId, ByVal riid, Len(rClsId)                  ' Copie le GUID de l'interface � structurer
        
        bIID = String$(MAX_GUIDLEN, 0)                              ' Alloue � l'avance le tableau d'octets
        Rc = StringFromGUID2(rClsId, VarPtr(bIID(0)), MAX_GUIDLEN)  ' Obtient la variable clsid depuis le GUID de structure
        Rc = InStr(1, bIID, vbNullChar) - 1                         ' Recherche les caract�res nuls de fin
        GetIIDFromPTR = Left$(UCase(bIID), Rc)                      ' Supprime les espace nuls en trop et convertit les caract�res en lettre majuscule pour comparaison
    End If
'------------------------------------------------------------
End Function
'------------------------------------------------------------
