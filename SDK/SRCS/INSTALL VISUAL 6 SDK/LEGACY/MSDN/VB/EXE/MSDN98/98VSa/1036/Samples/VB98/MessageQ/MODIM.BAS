Attribute VB_Name = "InsuraceModule"
Option Explicit
Global Const KEY_NAME = "Nom"
Global Const KEY_STREET = "Rue"
Global Const KEY_CITY = "Ville"
Global Const KEY_STATE = "�tat"
Global Const KEY_CLAIM = "R�clamation"
Global Const CLAIM_AUTO = "V�HICULE"
Global Const CLAIM_HOME = "MAISON"
Global Const CLAIM_BOAT = "BATEAU"
Global Const KEY_AMOUNT = "MontantR�clamation"

' ---------------------------------------------------
' Ceci pourrait �tre g�n�r� automatiquement par un Assistant
' ---------------------------------------------------
Global Const INSURANCE_CLAIMS_QUEUE_NAME = "sampleInsuranceClaimsQueue3"
Global Const INSURANCE_CLAIMS_QUEUE_LOCATION = ".\sampleInsuranceClaimsQueue3"

Global Const INSURANCE_ACCOUNTING_QUEUE_NAME = "sampleInsuranceAccountingQueue3"
Global Const INSURANCE_ACCOUNTING_QUEUE_LOCATION = ".\sampleInsuranceAccountingQueue3"

' ---------------------------------------------------
' Message d'erreur
' ---------------------------------------------------
Global Const ERROR_TEXT_RUNADMINTOOL = "La file de messages est inaccessible.  V�rifiez que vous avez bien cr�� les files n�cessaires en utilisant le projet 'Administer_Step0'."
