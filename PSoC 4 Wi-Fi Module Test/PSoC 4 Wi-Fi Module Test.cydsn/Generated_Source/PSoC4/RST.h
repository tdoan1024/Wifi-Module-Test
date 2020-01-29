/*******************************************************************************
* File Name: RST.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_RST_H) /* Pins RST_H */
#define CY_PINS_RST_H

#include "cytypes.h"
#include "cyfitter.h"
#include "RST_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} RST_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   RST_Read(void);
void    RST_Write(uint8 value);
uint8   RST_ReadDataReg(void);
#if defined(RST__PC) || (CY_PSOC4_4200L) 
    void    RST_SetDriveMode(uint8 mode);
#endif
void    RST_SetInterruptMode(uint16 position, uint16 mode);
uint8   RST_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void RST_Sleep(void); 
void RST_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(RST__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define RST_DRIVE_MODE_BITS        (3)
    #define RST_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - RST_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the RST_SetDriveMode() function.
         *  @{
         */
        #define RST_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define RST_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define RST_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define RST_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define RST_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define RST_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define RST_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define RST_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define RST_MASK               RST__MASK
#define RST_SHIFT              RST__SHIFT
#define RST_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RST_SetInterruptMode() function.
     *  @{
     */
        #define RST_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define RST_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define RST_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define RST_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(RST__SIO)
    #define RST_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(RST__PC) && (CY_PSOC4_4200L)
    #define RST_USBIO_ENABLE               ((uint32)0x80000000u)
    #define RST_USBIO_DISABLE              ((uint32)(~RST_USBIO_ENABLE))
    #define RST_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define RST_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define RST_USBIO_ENTER_SLEEP          ((uint32)((1u << RST_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << RST_USBIO_SUSPEND_DEL_SHIFT)))
    #define RST_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << RST_USBIO_SUSPEND_SHIFT)))
    #define RST_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << RST_USBIO_SUSPEND_DEL_SHIFT)))
    #define RST_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(RST__PC)
    /* Port Configuration */
    #define RST_PC                 (* (reg32 *) RST__PC)
#endif
/* Pin State */
#define RST_PS                     (* (reg32 *) RST__PS)
/* Data Register */
#define RST_DR                     (* (reg32 *) RST__DR)
/* Input Buffer Disable Override */
#define RST_INP_DIS                (* (reg32 *) RST__PC2)

/* Interrupt configuration Registers */
#define RST_INTCFG                 (* (reg32 *) RST__INTCFG)
#define RST_INTSTAT                (* (reg32 *) RST__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define RST_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(RST__SIO)
    #define RST_SIO_REG            (* (reg32 *) RST__SIO)
#endif /* (RST__SIO_CFG) */

/* USBIO registers */
#if !defined(RST__PC) && (CY_PSOC4_4200L)
    #define RST_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define RST_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define RST_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define RST_DRIVE_MODE_SHIFT       (0x00u)
#define RST_DRIVE_MODE_MASK        (0x07u << RST_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins RST_H */


/* [] END OF FILE */
