using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ScenChanger : MonoBehaviour
{
    public GameObject panel;
    string dataScenName;

    public void StartOpenScen(string name)
    {
        Time.timeScale = 1f;
        GameObject.Find("SoundManager").GetComponent<LerpSound>().LerpTo(0);
        GameObject.Find("Main Camera").GetComponent<LerpSound>().LerpTo(0);
        dataScenName = name;
        panel.SetActive(true);
        Invoke("OpenScene", 1f);
    }
    public void OpenURL(string url)
    {
        Application.OpenURL(url);
    }
    public void OpenScene()
    {
        SceneManager.LoadScene(dataScenName);
    }

    public void Exit()
    {
        Application.Quit();
    }
}
