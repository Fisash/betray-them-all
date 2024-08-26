using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[CreateAssetMenu(menuName = "Custom/WeaponPattern")]
public class WeaponPattern : ItemPattern
{
    public WeaponType type;
    public int minStartDamage;
    public int maxStartDamage;

    public int minStartCriticalChance;
    public int maxStartCriticalChance;

    public override string GetDescription(Item item)
    {
        Weapon weapon = (Weapon)item;
        string specialDescription = "ÓÐ: " + weapon.damage.ToString() + "\nÊÐÒ ÓÄÀÐ: " + weapon.criticalStrikeChance.ToString() + "%\n×ÀÐÛ:\n--íåò--";
        return baseDescription.Length > 0 ? baseDescription + "\n" + specialDescription : specialDescription;
    }
}
