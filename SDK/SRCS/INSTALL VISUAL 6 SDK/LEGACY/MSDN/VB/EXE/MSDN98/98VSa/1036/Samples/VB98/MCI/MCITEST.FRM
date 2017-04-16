VERSION 5.00
Begin VB.Form frmMCITest 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Application pour le contr�le MCI"
   ClientHeight    =   870
   ClientLeft      =   2040
   ClientTop       =   2040
   ClientWidth     =   4020
   Icon            =   "mcitest.frx":0000
   LinkMode        =   1  'Source
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   ScaleHeight     =   870
   ScaleWidth      =   4020
   Begin VB.Menu AL_DEVICES 
      Caption         =   "&P�riph�riques"
      Begin VB.Menu AI_CDAUDIO 
         Caption         =   "&CD Audio..."
      End
      Begin VB.Menu AI_SEQUENCER 
         Caption         =   "S�quenceur &MIDI..."
      End
      Begin VB.Menu AI_WAVEAUDIO 
         Caption         =   "&Son..."
      End
   End
   Begin VB.Menu AL_HELP 
      Caption         =   "&?"
      Begin VB.Menu AI_ABOUT 
         Caption         =   "&� propos de..."
      End
   End
End
Attribute VB_Name = "frmMCITest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub AI_ABOUT_Click()
    ' Affiche la bo�te de dialogue "� propos de...".
    ' Autorise les messages de Windows � �tre trait�s avant de
    ' d�finir le focus sur le bouton OK.
    frmAboutBox.Show
    WaitForEventsToFinish 50
    On Error Resume Next
    frmAboutBox.BTN_OK.SetFocus
End Sub

Private Sub AI_CDAUDIO_Click()
    frmCD.Show
    frmMCITest.Hide
    DisplayWarning
    
End Sub

Private Sub AI_SEQUENCER_Click()
    DialogCaption = " S�quenceur MIDI - "
    frmWave.Caption = "S�quenceur MIDI "
    
    ' D�finit la bo�te de dialogue Ouverture pour afficher les fichiers *.mid
    frmOpenDlg.dlgOpenFile.Filter = "Fichier MIDI (*.mid)|*.mid"
    frmWave.mciWave.DeviceType = "Sequencer"
    frmWave.Show
    frmMCITest.Hide
End Sub

Private Sub AI_WAVEAUDIO_Click()
    DialogCaption = "Son - "
    frmWave.Caption = "Son"
    
    ' D�finit la bo�te de dialogue Ouverture pour afficher les fichiers *.wav.
    frmOpenDlg.dlgOpenFile.Filter = "Fichier son (*.wav)|*.wav"
    frmWave.mciWave.DeviceType = "WaveAudio"
    frmWave.Show
    frmMCITest.Hide
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' D�charge la feuille OpenDlg cach�e.
    Unload frmOpenDlg
End Sub

