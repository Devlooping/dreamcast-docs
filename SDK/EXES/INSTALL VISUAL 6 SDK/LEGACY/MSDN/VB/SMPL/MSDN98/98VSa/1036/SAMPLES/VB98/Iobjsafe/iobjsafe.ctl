VERSION 5.00
Begin VB.UserControl UCObjectSafety 
   BackColor       =   &H00C00000&
   BorderStyle     =   1  'Fixed Single
   ClientHeight    =   525
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4170
   EditAtDesignTime=   -1  'True
   FillColor       =   &H00C00000&
   ForeColor       =   &H00C00000&
   ScaleHeight     =   525
   ScaleWidth      =   4170
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BorderStyle     =   1  'Fixed Single
      Caption         =   "IObjectSafety Interface Sample Control"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Left            =   120
      TabIndex        =   0
      Top             =   90
      Width           =   3900
   End
End
Attribute VB_Name = "UCObjectSafety"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
' ------------------------------------------------------------------------
'�Copyright � 1997 Microsoft Corporation. Tous droits r�serv�s.
' Vous disposez d'un droit accord� � titre gratuit d'utiliser, modifier, reproduire et ' distribuer, de quelque mani�re que ce soit,
' des extraits de Fichiers d'Applications Exemples. ' Vous reconnaissez �tre inform� et vous acceptez que Microsoft exclut toute garantie,
' obligation ' ou responsabilit� relatives � l'exercice du droit gratuit qui vous est accord� ci-dessus.
'-------------------------------------------------------------------------
Option Explicit
Implements IObjectSafety

Private m_Safety                As Boolean
Private m_fMakeSafeForScripting As Boolean

'------------------------------------------------------------
Private Sub IObjectSafety_GetInterfaceSafetyOptions(ByVal riid As Long, pdwSupportedOptions As Long, pdwEnabledOptions As Long)
'------------------------------------------------------------
    Dim IID         As String                           ' Cha�ne de l'ID de l'interface
'------------------------------------------------------------
    ' D�finit les fonctionnalit�s s�curis�es de l'objet pris en charge...
    pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER Or INTERFACESAFE_FOR_UNTRUSTED_DATA
                          
    IID = GetIIDFromPTR(riid)                           ' Extrait la cha�ne de l'ID de l'interface depuis le pointeur
    
    Select Case IID                                     ' D�termine l'interface qui demande les param�tres
    Case IID_IDispatch                                  ' Interface IDispatch.
        ' si ce contr�le est s�curis� pour initialisation _
          l'indicateur INTERFACESAFE_FOR_UNTRUSTED_DATA est alors d�nifi
        pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA    ' d�finit l'indicateur de fonctionnalit� activ�
        Exit Sub                                                ' quitte et l'op�ration a r�ussi
    Case IID_IPersistStorage, IID_IPersistStream, IID_IPersistPropertyBag
        ' si ce contr�le est s�curis� pour le traitement de script ou s'il peut �tre s�curis� pour_
        ' le traitement de script l'indicateur INTERFACESAFE_FOR_UNTRUSTED_CALLER est alors d�fini
        pdwEnabledOptions = pdwEnabledOptions Or INTERFACESAFE_FOR_UNTRUSTED_CALLER     ' D�finit l'indicateur de fonctionnalit� activ�
        Exit Sub                                        ' Op�ration r�ussie
    Case Else                                           ' Interface inconnue demand�e.
        Err.Raise E_NOINTERFACE                         ' Options s�curis�es demand�es pour l'interface ne sont pas prises en charge.
    End Select
        
    Err.Raise E_FAIL                                    ' Le param�tre s�curis� pour l'interface n'est pas pris en charge
'------------------------------------------------------------
End Sub
'------------------------------------------------------------

'------------------------------------------------------------
Private Sub IObjectSafety_SetInterfaceSafetyOptions(ByVal riid As Long, ByVal dwOptionsSetMask As Long, ByVal dwEnabledOptions As Long)
'------------------------------------------------------------
    Dim fSettings   As Long                             ' Indicateur de param�tres s�curis�s
    Dim IID         As String                           ' Cha�ne de l'ID de l'interface
'------------------------------------------------------------
    fSettings = (dwEnabledOptions And dwOptionsSetMask) ' Obtient l'indicateur des param�tres s�curis�s
    IID = GetIIDFromPTR(riid)                           ' Obtient la cha�ne de l'ID de l'interface depuis le pointeur
    
    Select Case IID                                     ' D�termine l'interface qui demande les param�tres
    Case IID_IDispatch                                  ' Interface IDispatch.
        ' ************************************************************
        ' Si ce contr�le n'est pas ou ne peut pas �tre s�curis� pour le traitement de script,
        ' la ligne de code suivante est mise en commentaire pour renvoyer E_FAIL
        ' ************************************************************
        ' Err.Raise E_FAIL                              ' Renvoie une erreur si le contr�le n'est pas s�curis� pour le traitement de script.
        
        If (fSettings = INTERFACESAFE_FOR_UNTRUSTED_CALLER) Then
        ' Si ce contr�le n'est pas consid�r� comme s�curis� pour le traitement de script, _
          il peut cependant �tre s�curis� pour le traitement de script en d�sactivant ses _
          fonctionnalit�s non s�curis�es.  Utilise cet indicateur pour indiquer que le contr�le requis doit �tre _
          s�curis� pour le traitement de script.
            m_fMakeSafeForScripting = True              ' Configure sur True, le contr�le doit �tre s�curis� pour le traitement de script.
            Exit Sub                                    ' op�ration r�ussie
        End If
    Case IID_IPersistStorage, IID_IPersistStream, IID_IPersistPropertyBag
        ' ************************************************************
        ' Si ce contr�le n'est jamais s�curis� pour l'initialisation,
        ' l'interface IObjectSafety ne devrait pas �tre ajout�e.
        ' ************************************************************
        If (fSettings = INTERFACESAFE_FOR_UNTRUSTED_DATA) Then
            ' Si ce contr�le est toujours s�curis� pour l'initialisation
            Exit Sub                                    ' Op�ration r�ussie
        End If
    Case Else                                           ' Interface requise inconnue.
        Err.Raise E_NOINTERFACE                         ' Options s�curis�es requises pour l'interface ne sont pas prises en charge.
    End Select
    
    Err.Raise E_FAIL                                    ' L'option s�curis�e pour une interface n'est pas prise en charge
'------------------------------------------------------------
End Sub
'------------------------------------------------------------

'------------------------------------------------------------
Public Property Get Safe() As Boolean
'------------------------------------------------------------
' Cette propri�t� est consid�r�e comme s�curis�e pour le traitement de script _
  et peut �tre utilis�e en toute s�curit� par un conteneur d'h�te.
'------------------------------------------------------------
    MsgBox "Appel de la m�thode:: Property Get Safe"
    Safe = m_Safety                                     ' Renvoie la valeur de donn�es bool�enne
'------------------------------------------------------------
End Property
'------------------------------------------------------------

'------------------------------------------------------------
Public Property Let Safe(ByVal IsSafe As Boolean)
'------------------------------------------------------------
' Cette propri�t� est consid�r�e comme s�curis�e pour le traitement de script _
  et peut �tre utilis�e en toute s�curit� par un conteneur d'h�te.
'------------------------------------------------------------
    MsgBox "Appel de la m�thode:: Property Let Safe"
    m_Safety = IsSafe                                   ' D�finit la valeur de donn�es bool�enne
'------------------------------------------------------------
End Property
'------------------------------------------------------------

'------------------------------------------------------------
Public Property Get UnSafe() As Boolean
' ------------------------------------------------------------
'  Cette propri�t� n'est pas s�curis�e pour le traitement de script
'  mais elle peut �tre requise pour s�curiser
'  par le conteneur d'h�te.  Si cette propri�t� doit �tre s�curis�e
'  cette fonction doit alors �tre d�sactiv�e losqu'elle est appel�e.
' ------------------------------------------------------------
    If m_fMakeSafeForScripting Then                     ' Objet requis doit-il �tre s�curis� ?
        Err.Raise E_FAIL                                ' Erreur renvoy�e : cette propri�t� n'est pas s�curis�e.
        Exit Property                                   ' Sortie
    Else
        MsgBox "Appel de la m�thode:: Property Get UnSafe"
        UnSafe = m_Safety                                 ' Renvoie la valeur de donn�es bool�enne
    End If
'------------------------------------------------------------
End Property
'------------------------------------------------------------

'------------------------------------------------------------
Public Property Let UnSafe(ByVal IsSafe As Boolean)
' ------------------------------------------------------------
'  Cette propri�t� n'est pas s�curis�e pour le traitement de script
'  elle peut cependant �tre requise pour s�curiser
'  par le conteneur d'h�te.  Si cette propri�t� doit �tre s�curis�e
'  cette fonction doit alors �tre d�sactiv�e losqu'elle est appel�e.
' ------------------------------------------------------------
    If m_fMakeSafeForScripting Then                     ' Objet requis doit-il �tre s�curis� ?
        Err.Raise E_FAIL                                ' Erreur renvoy�e : cette propri�t� n'est pas s�curis�e.
        Exit Property                                   ' Sortie
    Else
        MsgBox "Appel de la m�thode:: Property Let UnSafe"
        m_Safety = IsSafe                               ' Renvoie la valeur de donn�es bool�enne
    End If
'------------------------------------------------------------
End Property
'------------------------------------------------------------

'------------------------------------------------------------
Public Function ASafeFunctionToCall(ByVal lparm As Long) As Long
' ------------------------------------------------------------
'  Cette fonstion est consid�r�e comme s�curis�e pour le traitement de script
'  elle peut cependant �tre appel�e en toute s�curit� pour le traitement de script.
' ------------------------------------------------------------
    MsgBox "Appel de la m�thode:: Function ASafeFunctionToCall"
    ASafeFunctionToCall = 1                             ' Renvoie une valeur
'------------------------------------------------------------
End Function
'------------------------------------------------------------

'------------------------------------------------------------
Public Function AnUnSafeFunction(ByVal badparm As Long) As Long
' ------------------------------------------------------------
'  Cette fonction n'est pas consid�r�e comme s�curis�e pour le traitement de script
'  et ne doit en aucun cas �tre appel�e pour le traitement de script.  Le cas �ch�ant, une erreur d'ex�cution sera renvoy�e
'  pour d�sactiver cette fonction.
' ------------------------------------------------------------
    If m_fMakeSafeForScripting Then                     ' Contr�le requis doit-il �tre s�curis� ?
        Err.Raise E_FAIL                                ' Erreur renvoy�e : cette fonction n'est pas s�curis�e.
        Exit Function                                   ' Renvoie
    Else
        MsgBox "Appel de la m�thode:: Function AnUnSafeFunction"
        AnUnSafeFunction = 32                           ' Renvoie une valeur
    End If
' ------------------------------------------------------------
 End Function
' ------------------------------------------------------------
