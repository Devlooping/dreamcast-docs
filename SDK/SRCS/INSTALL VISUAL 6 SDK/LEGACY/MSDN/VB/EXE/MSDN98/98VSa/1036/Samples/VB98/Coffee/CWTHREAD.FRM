VERSION 5.00
Begin VB.Form frmThread 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Exemple de traitement multithread"
   ClientHeight    =   4950
   ClientLeft      =   4140
   ClientTop       =   1470
   ClientWidth     =   7095
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4950
   ScaleWidth      =   7095
   Begin VB.Timer tmrShort 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   2280
      Top             =   3840
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "&Annuler les t�ches longues"
      Height          =   375
      Left            =   240
      TabIndex        =   8
      Top             =   4440
      Width           =   2895
   End
   Begin VB.TextBox txtN 
      Height          =   375
      Left            =   2760
      MaxLength       =   1
      TabIndex        =   6
      Text            =   "4"
      Top             =   2160
      Width           =   375
   End
   Begin VB.CommandButton cmdNLong 
      Caption         =   "...avec &n t�ches longues"
      Height          =   375
      Left            =   240
      TabIndex        =   5
      Top             =   2160
      Width           =   2415
   End
   Begin VB.CommandButton cmdShortLong 
      Caption         =   "...avec t�ches &longues"
      Height          =   375
      Left            =   240
      TabIndex        =   4
      Top             =   1680
      Width           =   2895
   End
   Begin VB.CommandButton cmdShortOnly 
      Caption         =   "&T�ches courtes, en s�rie..."
      Height          =   375
      Left            =   240
      TabIndex        =   3
      Top             =   1200
      Width           =   2895
   End
   Begin VB.CommandButton cmdIDs 
      Caption         =   "Liste des &ID de thread"
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   720
      Width           =   2895
   End
   Begin VB.ListBox lstResults 
      Height          =   4545
      Left            =   3360
      TabIndex        =   1
      Top             =   120
      Width           =   3615
   End
   Begin VB.CommandButton cmdXThread 
      Caption         =   "&D�pr�ciation entre les threads"
      Height          =   375
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   2895
   End
   Begin VB.Label Label1 
      Caption         =   "Vos tests ici..."
      Height          =   255
      Left            =   240
      TabIndex        =   7
      Top             =   2760
      Width           =   2895
   End
End
Attribute VB_Name = "frmThread"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' L'exemple de traitement multithread:
'   - Affiche les ID des threads et le nombre
'       des objets d'une thread (ceci est
'       plus int�ressant si le nombre maximale
'       de threads est de 3 ou de 4 lors de la
'       compilation de MTCoffee.exe).
'   - Compare la d�pr�ciation entre les appels
'       effectu�s sur une m�me thread et ceux
'       effectu�s entre plusieurs threads.
'   - Fournit le temps par it�ration pour les
'       t�ches courtes en s�rie (Voir l'objet
'       Coffee d�fini dans MTCoffee.cls).
'   - Fournit le temps par it�ration pour une
'       t�che longue, et des t�ches courtes en
'       s�rie s'ex�cutant en m�me temps.
'   - Fournit le temps par it�ration pour un
'       nombre de t�ches longues, et des t�ches
'       courtes en s�rie s'ex�cutant en m�me temps.
'
' Ceci constitue une base � laquelle vous pouvez
'   adjoindre vos propres tests. Il peut donc �tre
'   int�ressant de comparer le comportement des
'   t�ches qui bloquent (telles que les requ�tes
'   aux bases de donn�es sur des ordinateurs
'   distants, ou les transferts de fichiers
'   importants). Sur un ordinateur � un seul
'   processeur, ce type de t�ches se comporte
'   mieux que les t�ches qui sont propos�es
'   dans les exemples ci-dessus, qui exigent
'   d'importantes ressources de traitement.
'   Les threads qui ex�cutent ce genre de
'   t�ches entrent en conflit pour utiliser
'   l'unique processeur de la machine et
'   leur performance d�cro�t � mesure que le
'   nombre de threads actives augmente.
'
' Pour plus d'informations, reportez-vous �
'   la section "�volutivit� et traitement
'   Multithread", du chapitre "Construction
'   de composants de code" des Manuels en ligne.
 
' Ces constantes contr�lent la taille relative
'   d'une t�che longue et d'une t�che courte.
'   Vous devrez peut-�tre les ajuster � la
'   vitesse de votre processeur.
Const SHORTTASKSIZE = 50000
Const LONGTASKSIZE = 2000000

' Tableau d'objets Coffee.
Private macfe(1 To 20) As Coffee

' Collection d'objets CoffeeTracker.
Public CoffeeTrackers As New Collection

' Indicateur d'annulation.
Public CancelAll As Boolean

' Nombre de t�ches courtes � ex�cuter.
Private mintHowManyShort As Integer

Private Sub cmdCancel_Click()
    ' Lorsque l'utilisateur clique sur Annuler,
    '   d�finit un indicateur que l'objet CoffeeTracker
    '   peut utiliser pour annuler toutes les
    '   t�ches longues lorsqu'elles d�clenchent
    '   l'�v�nement Progress suivant.
    CancelAll = True
End Sub

' Ex�cute quelques t�ches courtes en s�rie,
'   pour avoir un aper�u de leur vitesse
'   lorsqu'il n'y a pas de conflit pour
'   l'utilisation du processeur.
Private Sub cmdShortOnly_Click()
    lstResults.Clear
    CancelAll = False
    mintHowManyShort = 10
    tmrShort.Interval = 250
    tmrShort.Enabled = True
End Sub

' D�marre l'ex�cution d'une t�che longue,
'   puis l'ex�cution de t�ches courtes en
'   s�rie avant la fin de la t�che longue.
Private Sub cmdShortLong_Click()
    Dim cft As CoffeeTracker
    
    lstResults.Clear
    CancelAll = False
    Set cft = NewTracker(macfe(1).ThreadID, LONGTASKSIZE)
    Set cft.Coffee = macfe(1)
    Call macfe(1).StartLongTask(LONGTASKSIZE)
    '
    ' Met en ligne quelques t�ches courtes �
    '   ex�cuter (une tous les quarts de seconde)
    '   pendant l'ex�cution de la t�che.
    mintHowManyShort = 10
    tmrShort.Enabled = True
    tmrShort.Interval = 250
End Sub

' D�marre N t�ches longues (1 - 9), puis ex�cute
'   une s�rie de t�ches courtes.
Private Sub cmdNLong_Click()
    Dim intCt As Integer
    Dim cft As CoffeeTracker
    
    lstResults.Clear
    CancelAll = False
    For intCt = 1 To CLng("0" & txtN)
        Set cft = NewTracker(macfe(intCt).ThreadID, LONGTASKSIZE)
        Set cft.Coffee = macfe(intCt)
        Call macfe(intCt).StartLongTask(LONGTASKSIZE)
    Next
    '
    ' Met en ligne quelques t�ches courtes
    '   � ex�cuter pendant l'ex�cution de
    '   la t�che longue.
    mintHowManyShort = 10 + CLng("0" & txtN)
    tmrShort.Enabled = True
    tmrShort.Interval = 250
End Sub

' Compare la d�pr�ciation des appels pour
'   des appels � un objet de la m�me thread,
'   et pour des appels � un objet d'une
'   autre thread.
'
Private Sub cmdXThread_Click()
    Dim cfeSame As Coffee
    Dim cfeNew As Coffee
    
    cmdXThread.Caption = "Ex�cution..."
    cmdXThread.Enabled = False
    
    ' Cr�e un objet Coffee sur une autre thread.
    Set cfeNew = macfe(10).GetCoffeeOnNewThread
    ' En cas de regroupement de threads, une
    '   thread diff�rente sera obtenue la
    '   deuxi�me fois, sauf s'il n'existe qu'une
    '   thread dans le regroupement.
    If cfeNew.ThreadID = macfe(10).ThreadID Then
        Set cfeNew = macfe(10).GetCoffeeOnNewThread
        If cfeNew.ThreadID = macfe(10).ThreadID Then
            MsgBox "Impossible de comparer les appels sur une m�me thread avec ceux effectu�s entre threads; impossible d'obtenir un objet sur une autre thread."
            Exit Sub
        End If
    End If
    '
    ' Cr�e un objet Coffee sur la m�me thread.
    Set cfeSame = macfe(10).GetCoffeeOnSameThread
    
    ' Utilise les derniers objets Coffee
    '   pour effectuer le test.
    MsgBox "M�me thread:  " & macfe(10).CallAnotherCoffee(cfeSame) & " sec/appel" & vbCrLf _
        & "Interthread:  " & macfe(10).CallAnotherCoffee(cfeNew) & " sec/appel"
    
    cmdXThread.Caption = "D�pr�ciation interthreads"
    cmdXThread.Enabled = True
    '
    ' Les objets Coffee cr��s pour ce test
    '   se terminent lorsque cfeNew et cfeSame
    '   se trouvent hors de port�e � la fin
    '   de cette proc�dure.
End Sub

' Liste les ID des thread des objets Coffee
'   cr��s lors du chargement de cette feuille.
'
Private Sub cmdIDs_Click()
    Dim intCt As Integer
    lstResults.Clear
    For intCt = 1 To 10
        lstResults.AddItem macfe(intCt).ThreadID _
            & "  (" & macfe(intCt).NumberOnThread & " sur la thread)"
    Next
End Sub

' Cr�e une s�rie d'objets Coffee, chacun sur sa
'   propre thread. Si vous recompilez MTCoffee
'   lorsque le nombre maximum de threads est
'   inf�rieur � 10, certains partageront la
'   thread et l'�tat global.
'
Private Sub Form_Load()
    Dim intCt As Integer
    For intCt = 1 To 10
        Set macfe(intCt) = New Coffee
    Next
    Form1.cmdMT.Enabled = True
    Form1.cmdMT.MousePointer = vbDefault
End Sub

Private Sub Text1_KeyPress(KeyAscii As Integer)
    Select Case KeyAscii
        Case 48 To 57, 8
        Case Else
            Beep
            KeyAscii = 0
    End Select
End Sub

' Fournit des cl�s uniques pour les objets CoffeeTracker.
'
Private Function NewKey() As String
    Static lngLastKey As Long
    lngLastKey = lngLastKey + 1
    NewKey = "K" & lngLastKey
End Function

' Ajoute un nouvel objet CoffeeTracker. En fait, cela
'   devrait �tre une m�thode d'une classe de la
'   collection CoffeeTrackers.
'
Private Function NewTracker(ByVal ThreadID As Long, _
        ByVal Size As Long) As CoffeeTracker
        
    Dim cft As New CoffeeTracker
    '
    ' Place en m�moire cache l'ID de la thread
    '   de l'objet Coffee dont le traqueur
    '   garde la trace.
    cft.ThreadID = ThreadID
    '
    ' D�finit la taille de la t�che affect�e �
    '   l'objet Coffee dont le traqueur garde
    '   la trace.
    cft.Size = Size
    '
    ' Donne au traqueur une cl� unique pour
    '   la collection.
    cft.ID = NewKey
    '
    ' Met le nouveau traqueur dans une collection.
    CoffeeTrackers.Add cft, cft.ID
    '
    ' Renvoie une r�f�rence au nouveau traqueur.
    Set NewTracker = cft
End Function

' La minuterie est utilis�e pour d�marrer une
'   s�rie de t�ches courtes, � intervalles
'   r�guliers, en utilisant un objet Coffee
'   (en fait, une thread unique). Avant de
'   d�marrer une nouvelle t�che, elle v�rifie
'   si la t�che pr�c�dente est termin�e.
'
' Si vous ex�cutez le projet MTCoffee dans l'environnement
'   de d�veloppement, mettez en commentaire le code qui
'   v�rifie si la t�che pr�c�dente est termin�e.
'
Private Sub tmrShort_Timer()
    Static intCt As Integer
    Static strWaitingFor As String
    Dim cft As CoffeeTracker
    
    ' Si la variable strWaitingFor contient une cl� de l'objet
    ' CoffeeTracker, l'objet Coffee ex�cute (ou ex�cutait)
    ' une t�che en �tant surveill� par l'objet CoffeeTracker.
    If strWaitingFor <> "" Then
        On Error Resume Next
        '
        ' Si l'objet CoffeeTracker attendu
        '   ne fait plus partie de la
        '   collection, une erreur se produit;
        '   cela signifie qu'il convient d'en
        '   d�marrer un autre.
        Set cft = CoffeeTrackers(strWaitingFor)
        If Err.Number = 0 Then Exit Sub
    Else
        ' Si la variable de cha�ne statique est
        '   vide, la s�rie de t�ches courtes
        '   pr�c�dentes est termin�e. Une nouvelle
        '   s�rie commence; le compteur statique
        '   est donc r�initialis� (intCt).
        intCt = 0
    End If
    
    If Not CancelAll Then
        intCt = intCt + 1
        '
        ' Cr�e un objet CoffeeTracker attendant
        '   la demande � terminer.
        Set cft = NewTracker(macfe(10).ThreadID, SHORTTASKSIZE)
        '
        ' Donne � l'objet CoffeeTracker son
        '   objet Coffee � surveiller.
        Set cft.Coffee = macfe(10)
        '
        ' D�marre la t�che.
        Call macfe(10).StartLongTask(SHORTTASKSIZE)
        '
        ' Se pr�pare � attendre l'objet
        '   CoffeeTracker.
        strWaitingFor = cft.ID
    End If
    '
    ' V�rifie si la s�rie de t�ches courtes
    '   est termin�e ou si elle a �t� interrompue
    '   par un clic sur le bouton Annuler:
    If (intCt >= mintHowManyShort) Or CancelAll Then
        intCt = 0
        tmrShort.Enabled = False
        strWaitingFor = ""
    End If
End Sub
