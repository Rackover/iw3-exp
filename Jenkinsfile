#!groovy

import groovy.transform.Field

@Field def configurations = [
	"Debug": [
		WorkspaceID: "build@debug",
		StashName: "bin-debug",
		MSBuildConfiguration: "Debug",
		PremakeArgs: "",
		Archive: true,
	],
	"Release": [
		WorkspaceID: "build@release",
		StashName: "bin-release",
		MSBuildConfiguration: "Release",
		PremakeArgs: "",
		Archive: true,
	],
].collect {k, v -> [k, v]}

def jobWorkspace(id, f) {
	ws("workspace/${env.JOB_NAME.replaceAll(/[%$]/, "_")}@$id", f)
}

def useShippedPremake(f) {
	def premakeHome = "${pwd()}\\tools"

	withEnv(["PATH+=${premakeHome}"], f)
}

def powershellBat(cmd) {
	def fileName = "${pwd tmp: true}/powershell.ps1"

	writeFile file: fileName, text: cmd

	bat "powershell -ExecutionPolicy unrestricted \"${fileName}\""
}

// This will build the IW4x client.
// We need a Windows Server with Visual Studio 2015, Premake5 and Git on it.
def doBuild(cfg) {
	retry(5) {
		checkout scm
	}

	useShippedPremake {
		def outputDir = pwd()
		def msbuild = tool "Microsoft.NET MSBuild 15.0"
		bat "premake5 vs2017 ${cfg.PremakeArgs}"
		bat "\"${msbuild}\" build\\iw3-exp.sln \"/p:OutDir=$outputDir\\\\\" \"/p:Configuration=${cfg.MSBuildConfiguration}\""
	}

	stash name: "${cfg.StashName}", includes: "*.dll,*.pdb"
}

// Job properties
properties([
	[$class: "GitLabConnectionProperty", gitLabConnection: "dekart811"]
])

gitlabBuilds(builds: [
	//"Checkout & Versioning",
	"Build",
	"Archiving"
]) {
	// First though let's give this build a proper name
	/*stage("Checkout & Versioning") {
		gitlabCommitStatus("Checkout & Versioning") {
			node("windows") {
				jobWorkspace("versioning") {
					retry(5) {
						checkout scm
					}

					useShippedPremake {
						def version = bat(returnStdout: true, script: '@premake5 version').split("\r?\n")[1]

						currentBuild.setDisplayName "$version (#${env.BUILD_NUMBER})"
					}
				}
			}
		}
	}*/

	// For each available configuration generate a normal build and a unit test build.
	stage("Build") {
		gitlabCommitStatus("Build") {
			node("windows") {
				jobWorkspace("build@executable") {
					retry(5) {
						checkout scm
					}

					// Retry in case of network failures
					retry(5) {
						powershellBat "./res/install-dependencies.ps1"
					}
					powershellBat "./res/generate.ps1"

					stash name: "bin-executable", includes: "*.exe"
				}
			}

			def executions = [:]
			for (int i = 0; i < configurations.size(); i++) {
				def entry = configurations[i]

				def configName = entry[0]
				def config = entry[1]

				executions[configName] = {
					node("windows") {
						jobWorkspace(config.WorkspaceID) {
							doBuild(config)
						}
					}
				}
			}
			parallel executions
		}
	}

	// Collect all the binaries and give each configuration its own subfolder
	stage("Archiving") {
		gitlabCommitStatus("Archiving") {
			node("windows") { // any node will do
				jobWorkspace("archiving") {
					try {
						for (int i = 0; i < configurations.size(); i++) {
							def entry = configurations[i]

							def configName = entry[0]
							def config = entry[1]

							if (config.Archive) {
								dir(configName) {
									unstash config.StashName
								}
							}
						}

						unstash "bin-executable"
						archiveArtifacts artifacts: "**/*.dll,**/*.pdb,*.exe", fingerprint: true
					} finally {
						deleteDir()
					}
				}
			}
		}
	}
}
