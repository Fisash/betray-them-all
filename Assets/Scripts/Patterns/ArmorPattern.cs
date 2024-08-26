using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[CreateAssetMenu(menuName = "Custom/ArmorPattern")]
public class ArmorPattern : ItemPattern
{
    public ArmorType type;
    public int minStartProtection;
    public int maxStartProtection;

    public int minStartEvasionChance;
    public int maxStartEvasionChance;

    public override string GetDescription(Item item)
    {
        Armor armor = (Armor)item;
        string specialDescription = "ÇÙ: " + armor.protection.ToString() + "\nÓÊË: " + armor.evasionChance.ToString() + "%\n×ÀÐÛ:\n--íåò--";
        return baseDescription.Length > 0 ? baseDescription + "\n" + specialDescription : specialDescription;
    }
}
