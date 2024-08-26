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
        string specialDescription = "��: " + weapon.damage.ToString() + "\n��� ����: " + weapon.criticalStrikeChance.ToString() + "%\n����:\n--���--";
        return baseDescription.Length > 0 ? baseDescription + "\n" + specialDescription : specialDescription;
    }
}
